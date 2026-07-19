// IE11-safe JS only: no let/const, no arrow fns, no fetch,
// no URLSearchParams, no template literals.

function qs(name) {
    var m = new RegExp('[?&]' + name + '=([^&]*)').exec(window.location.search);
    return m ? decodeURIComponent(m[1].replace(/\+/g, ' ')) : '';
}

function byId(id) { return document.getElementById(id); }

// Populate hidden OAuth passthrough fields from the query string
byId('client_id').value     = qs('client_id');
byId('state').value         = qs('state');
byId('redirect_uri').value  = qs('redirect_uri');
byId('response_type').value = qs('response_type') || 'code';

// Fake session id for terminal flavor
byId('sessionId').innerHTML =
    Math.floor(Math.random() * 900000 + 100000).toString();

// Focus ring on the bordered input-row wrapper (IE11 has no :focus-within)
function wireFocus(inputId, rowId) {
    var input = byId(inputId);
    var row = byId(rowId);
    input.onfocus = function () { row.className = 'input-row focus'; };
    input.onblur  = function () { row.className = 'input-row'; };
}
wireFocus('username', 'userRow');
wireFocus('password', 'passRow');

// Basic client-side validation before submit
byId('loginForm').onsubmit = function (e) {
    var u = byId('username').value.replace(/^\s+|\s+$/g, '');
    var p = byId('password').value;
    var err = byId('errorBox');

    if (u === '' || p === '') {
    err.className = 'error show';
    err.innerHTML = 'ACCESS DENIED &mdash; OPERATOR ID AND ACCESS KEY REQUIRED';
    if (e && e.preventDefault) { e.preventDefault(); } else { window.event.returnValue = false; }
    return false;
    }

    err.className = 'error';
    return true;
};
