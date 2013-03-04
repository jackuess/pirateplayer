.pragma library

String.prototype.startsWith = function (s) {
    if (this.substr(0, s.length) === s)
        return true;
    else
        return false;
}
String.prototype.slim = function () {
    var r = this.replace(/\n/g, " ");
    return r.replace(/\s{2,}/g, "").trim();
}
Number.prototype.mod = function(n) {
    return ((this%n)+n)%n;
}
