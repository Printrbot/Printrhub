(function(){
"use strict";

// <swd-begin> initiates connection with the target if necessary.
//   When indeterminate, it is hidden.
//   If disconnected, the entire contents are replaced with an error
//   If successful, the contents are shown.
//   Also, optional info is written to the 'result-element'
//   If there's a 'loading-element', it's hidden after loading finishes.

var TargetInfo = document.registerElement('swd-begin', {
    prototype: Object.create(HTMLElement.prototype, {

        createdCallback: { value: function() {
            var el = this;
            var req = new XMLHttpRequest();
            el.style.display = 'none';
            el.api = el.getAttribute('api') || '/api';
            req.open('GET', el.api + '/begin');
            req.addEventListener('loadend', function () {
                el.render(JSON.parse(req.responseText));
            });
            req.send();
        }},

        render: { value: function(json) {
            var loadingAttr = this.attributes['loading-element'];
            if (loadingAttr) {
                document.getElementById(loadingAttr.value).style.display = 'none';
            }

            if (json.connected) {
                var resultsAttr = this.attributes['result-element'];
                if (resultsAttr) {
                    document.getElementById(resultsAttr.value).innerHTML = `
                        Connected to the ARM debug port. <br/>
                        Processor IDCODE: <b>${toHex32(json.idcode)}</b> <br/>
                        Detected extensions: ${json.detected} <br/>
                    `;
                }
            } else {
                this.innerHTML = `
                    Unfortunately, <br/>
                    I failed to connect to the debug port. <br/>
                    Check your wiring maybe? <br/>
                `;
            }
            this.style.display = null;
        }}
    })
});

// <swd-async-action> is a modified hyperlink that shows pass/fail asynchronously

var AsyncAction = document.registerElement('swd-async-action', {
    extends: 'a',
    prototype: Object.create(HTMLAnchorElement.prototype, {

        createdCallback: { value: function() {
            var el = this;

            this.addEventListener('click', function (evt) {
                el.action();
                evt.preventDefault();
            });

            this.resultElement = document.createElement('span');
            this.parentNode.insertBefore(this.resultElement, this.nextSibling);
            this.resultElement.className = 'result hidden';
        }},

        action: { value: function(json) {
            var el = this;
            var req = new XMLHttpRequest();

            el.resultElement.className = 'result hidden';
            el.resultElement.textContent = '';

            req.open('GET', this.href);
            req.addEventListener('loadend', function () {
                var response = JSON.parse(req.responseText);
                el.resultElement.textContent = response ? ' OK! ' : ' failed ';
                el.resultElement.className = 'result visible';
                setTimeout( function() {
                    el.resultElement.className = 'result hidden';
                }, 300);
            });
            req.send();
        }}
    })
});

// <swd-hexedit> is an editable hex dump with live updates

var Hexedit = document.registerElement('swd-hexedit', {
    prototype: Object.create(HTMLElement.prototype, {

        createdCallback: { value: function() {
            this.navigation = this.getAttribute('navigation') == 'true';
            this.headings = this.getAttribute('headings') != 'false';
            this.src = this.getAttribute('src') || '/api/mem';
            this.addr = parseInt(this.getAttribute('addr'), 0) || 0;
            this.count = parseInt(this.getAttribute('count'), 0) || 1024;
            this.columns = parseInt(this.getAttribute('columns'), 0) || 8;
            this.render();

            if (this.navigation) {
                // Render automaticaly when the nav parameters change
                var el = this;
                window.addEventListener('hashchange', function() {
                    el.render();
                });
            }
        }},

        render: { value: function() {
            // Keep nav parameters fresh on each render
            if (this.navigation) {
                var params = getHashParams();
                if ('addr' in params) this.addr = parseInt(params['addr'], 0);
                if ('count' in params) this.count = parseInt(params['count'], 0);
                if ('columns' in params) this.columns = parseInt(params['columns'], 0);
            }

            // Hex dump body, made of <swd-hexword> elements
            var parts = [];
            for (var i = 0, addr = this.addr; i < this.count;) {
                parts.push('<div>')
                if (this.headings) {
                    parts.push(toHex32(addr) + ':');
                }
                for (var col = 0; i < this.count && col < this.columns; col++) {
                    parts.push(` <span is="swd-hexword" addr="${addr}" src="${encodeURI(this.src)}"></span>`);
                    addr += 4;
                    i++;
                }
                parts.push('</div>');
            }
            this.innerHTML = parts.join('');
        }}
    })
});

// <swd-hexnav> is a navigation link for use with <swd-hexedit navigation="true">.
// The 'nav-step' parameter indicates how many bytes to step forward or backward in memory

var Hexnav = document.registerElement('swd-hexnav', {
    extends: 'a',
    prototype: Object.create(HTMLAnchorElement.prototype, {

        createdCallback: { value: function() {
            var el = this;
            this.addEventListener('click', function (evt) {
                el.action();
                evt.preventDefault();
            });
        }},

        action: { value: function(json) {
            var el = this;
            var step = parseInt(this.getAttribute('nav-step'), 0) || 1;
            var addr = parseInt(getHashParams()['addr']) || 0;
            var newAddr = Math.max(addr + step, 0);
            setHashParam('addr', `0x${toHex32(newAddr)}`);
        }}
    })
});

// The internal RefreshController trackskeeps track of
// all automatically updating values within a single window.

var RefreshController = (function() {

    var maxWordsPerRequest = 64;
    var minimumUpdateIntervalMillis = 100;

    // The Set can quickly keep track of the set of elements
    // without worrying about sorting order, then we lazily create
    // an index sorted by source URL first, then by ascending address.

    var elementSet = new Set();
    var elementIndex = null;

    function makeIndex() {
        elementIndex = Array.from(elementSet);
        elementIndex.sort(function (a, b) {
            if (a.src > b.src) return 1;
            if (a.src < b.src) return -1;
            return a.addr - b.addr;
        });
    }

    // We have a lot of elements that need to be tested for
    // visibility quickly, so this is a visibility tester that
    // caches element locations indefinitely and caches
    // document scroll location once per invocation.

    function cachedVisibilityTester() {
        if (document.hidden) {
            // The whole document is obscured or minimized
            return function() { return false; }
        }

        var bodyTop = document.body.getBoundingClientRect().top;
        var winHeight = document.documentElement.clientHeight;

        return function(element) {
            if (!element.cache) {
                var r = element.getBoundingClientRect();
                if (r && r.width && r.height) {
                    element.cache = {
                        top: r.top - bodyTop,
                        bottom: r.bottom - bodyTop
                    };
                } else {
                    // Not (yet) visible; try again later.
                    return false;
                }
            }
            return (element.cache.bottom + bodyTop >= 0 &&
                    element.cache.top + bodyTop <= winHeight);
        }
    }

    // The refreshes are organized into cycles. At the beginning
    // of each cycle, we take stock of what needs refreshing,
    // and store a list. It usually takes multiple HTTP requests
    // to get everything, so this 'cyclePending' list keeps track
    // of just elements that need to be looked at before the next
    // cycle starts.
    //
    // Cycles also end any time the window scrolls, or new elements
    // are registered or unregistered.

    var cyclePending = null;
    var currentRequest = null;

    window.addEventListener('scroll', function() {
        cyclePending = null;
        if (!currentRequest) {
            // In case we scrolled to an area with nothing visible
            beginRequest();
        }
    });

    function beginCycle() {
        // Walk the index looking for visible items.
        // Invisible elements become stale, and visible elements become part of the cycle.

        if (!elementIndex) {
            makeIndex();
        }

        var isVisible = cachedVisibilityTester();
        cyclePending = [];

        for (var el of elementIndex) {
            if (isVisible(el)) {
                cyclePending.push(el);
            } else {
                // Invisible and no longer updating; mark as stale
                el.className = 'mem-stale'
            }
        }
    }

    function beginRequest() {
        // Send another HTTP request, if we can.

        if (!cyclePending || !cyclePending.length) {
            beginCycle();
        }
        if (!cyclePending || !cyclePending.length) {
            // Nothing visible. If a new element is
            // added or scrolls into view, we'll be notified and retry.
            return;
        }

        // The cyclePending list is sorted by ascending address, but it may
        // have duplicates. Look for the first contiguous group of addresses,
        // to make the next request.

        var src = cyclePending[0].src;
        var firstAddr = cyclePending[0].addr;
        var wordCount = 0;
        var elementCount = 0;
        var elementsForEachWord = [];

        var now = Date.now();
        for (var el of cyclePending) {
            if (wordCount >= maxWordsPerRequest) {
                // Enough words
                break;
            }
            if (el.renderTimestamp && (now - el.renderTimestamp) < minimumUpdateIntervalMillis) {
                // This word has already been updated very recently; skip it on this cycle
                elementCount++;
                continue;
            }
            if (el.src != src) {
                // Different source URLs, can't combine
                break;
            }

            var nextAddr = firstAddr + wordCount * 4;

            if (wordCount && el.addr == nextAddr - 4) {
                // Oh, this is a duplicate address. Request it only once, and broadcast to all interested elements
                el.className = 'mem-loading';
                elementCount++;
                elementsForEachWord[wordCount-1].push(el);

            } else if (el.addr == nextAddr) {
                // Load another word contiguously. This case also handles the first word.
                el.className = 'mem-loading';
                elementCount++;
                elementsForEachWord.push([ el ]);
                wordCount++;

            } else {
                // Addresses stopped being contiguous, finish collecting now.
                break;
            }
        }

        // Dequeue from the pending list all at once
        cyclePending = cyclePending.slice(elementCount);

        var req = new XMLHttpRequest();
        currentRequest = req;
        var url = `${src}/read?addr=0x${toHex32(firstAddr)}&count=${wordCount}`;

        req.open('GET', url);
        req.addEventListener('loadend', function () {
            currentRequest = null;
            beginRequest();

            var response = JSON.parse(req.responseText);
            for (var i = 0; i < elementsForEachWord.length; i++) {
                for (var el of elementsForEachWord[i]) {
                    el.render(response[i]);
                }
            }
        });
        req.send();
    }

    var windowLoaded = false;
    window.addEventListener('load', function () {

        // Start allowing our own requests after the page loads.
        windowLoaded = true;

        // Normally the request cycle is maintained explicitly
        // and kickstarted immediately by scroll events,
        // but there are still some edge cases we don't catch.
        // For this, we poll.

        function poll() {
            if (windowLoaded) {
                if (!currentRequest) {
                    beginRequest();
                }
                setTimeout(poll, 250);
            }
        }

        // Issue the first request if we need to, and set the timer
        poll();
    });
    window.addEventListener('unload', function () {
        // Stop requests ASAP when navigating away
        windowLoaded = false;
        if (currentRequest) {
            currentRequest.abort();
        }
    });

    return {
        // Start refreshing an <swd-hexword> element
        register: function (element) {
            elementIndex = null;
            cyclePending = null;
            elementSet.add(element);
            element.cache = null;
            if (windowLoaded && !currentRequest) {
                beginRequest();
            }
        },

        // Forget about an element
        unregister: function (element) {
            if (elementSet.delete(element)) {
                elementIndex = null;
                cyclePending = null;
            }
        }
    };
})();

// <swd-hexword> is a single live-updating editable hex word

var Hexword = document.registerElement('swd-hexword', {
    extends: 'span',
    prototype: Object.create(HTMLSpanElement.prototype, {

        createdCallback: { value: function() {
            this.markStale();
            this.addr = parseInt(this.getAttribute('addr'), 0);
            this.src = this.getAttribute('src') || '/api/mem';
            this.contentEditable = true;
            this.savedValue = null;
            this.renderTimestamp = null;
            this.updateOnBlur = false;

            this.addEventListener('keydown', function(event) {
                var el = event.target;
                if (event.keyCode == 13) {
                    // This is enter or shift+enter.
                    // Send the value immediately, even if it hasn't changed,
                    // but don't insert a newline.
                    event.preventDefault();
                    el.savedValue = null;
                    el.updateValue();
                    el.selectAll();
                }
            });

            this.addEventListener('input', function(event) {
                var el = event.target;
                if (el == document.activeElement) {
                    el.updateOnBlur = true;
                } else {
                    el.updateValue();
                }
            });

            this.addEventListener('focus', function(evt) {
                // Select the whole word, for convenient keyboard editing
                evt.target.selectAll();
            });

            this.addEventListener('blur', function(evt) {
                // Implicit send if the value changed when navigating away
                var el = event.target;
                if (el.updateOnBlur) {
                    el.updateOnBlur = false;
                    el.updateValue();
                }
            });
        }},

        attachedCallback: {value: function() {
            RefreshController.register(this);
        }},

        detachedCallback: {value: function() {
            RefreshController.unregister(this);
        }},

        selectAll: { value: function() {
            var el = this;
            requestAnimationFrame(function() {
                var range = document.createRange();
                range.selectNodeContents(el);
                var sel = window.getSelection();
                sel.removeAllRanges();
                sel.addRange(range);
            });
        }},

        markStale: { value: function() {
            // The valid is unknown for now
            this.textContent = '........';
            this.className = 'mem-stale';
            this.renderTimestamp = null;
        }},

        render: { value: function(word) {
            // We have some memory or 'null' as an error marker
            var oldTimestamp = this.renderTimestamp;
            this.renderTimestamp = Date.now();

            if (this == document.activeElement) {
                // Don't change while we're being edited
                return;
            }

            if (word == null) {
                this.textContent = '////////';
                this.className = 'mem-error';
            } else {
                var textValue = toHex32(word);
                var oldValue = this.textContent;
                this.textContent = textValue;
                if (oldTimestamp == null) {
                    // First read after it was invalid
                    this.className = 'mem-okay';
                } else if (oldValue == textValue) {
                    this.className = 'mem-stable';
                } else {
                    this.className = 'mem-changing';
                }
            }
        }},

        updateValue: { value: function() {
            // Clean up the entered hex value, and transmit if it doesn't match savedValue

            var oldValue = this.savedValue;
            var newValue = parseInt(this.textContent, 16);
            if (isNaN(newValue)) {
                newValue = oldValue || 0;
            }

            this.textContent = toHex32(newValue);
            this.savedValue = newValue;

            if (newValue != oldValue) {
                this.className = 'mem-pending';
                this.textContent = toHex32(newValue);

                // Asynchronous write. Usually results will be apparent due to the
                // periodic read cycle, but we also try to keep the style updated
                // to indicate confirmed compvarion of the memory store operation.

                var url = `${this.src}/write?0x${toHex32(this.addr)}=0x${toHex32(newValue)}`;
                var el = this;
                var req = new XMLHttpRequest();
                req.open('GET', url);
                req.addEventListener('loadend', function () {
                    var result = JSON.parse(req.responseText)[0].result;
                    if (el.className == 'mem-pending') {
                        el.className = result ? 'mem-okay' : 'mem-error';
                    }
                });
                req.send();
            }
        }}
    })
});


function getHashParams() {
    // Parse key-value params from the fragment portion of the URI.
    // http://stackoverflow.com/a/4198132

    var hashParams = {};
    var e,
        a = /\+/g,  // Regex for replacing addition symbol with a space
        r = /([^&;=]+)=?([^&;]*)/g,
        d = function (s) { return decodeURIComponent(s.replace(a, ' ')); },
        q = window.location.hash.substring(1);

    while (e = r.exec(q)) {
       hashParams[d(e[1])] = d(e[2]);
    }
    return hashParams;
}

function setHashParam(key, value) {
    // Change one value from the fragment portion of the URI, leaving the others.

    var parts = window.location.hash.substring(1).split('&');
    var newPart = `${key}=${encodeURIComponent(value)}`;

    for (var i = 0; i < parts.length; i++) {
        if (parts[i].startsWith(key + '=')) {
            // Replace an existing key
            parts[i] = newPart;
            newPart = null;
            break;
        }
    }
    if (newPart) {
        // Append a new key
        parts.push(newPart);
    }
    
    window.location.hash = '#' + parts.join('&');
}

function toHex32(value) {
    return ('00000000' + value.toString(16)).slice(-8);
}

})();
