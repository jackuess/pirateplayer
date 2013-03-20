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

String.prototype.decodeHTMLEntities = function () {
    return this.replace(/&ouml;/g, "ö")
                .replace(/&Ouml;/g, "Ö")
                .replace(/&auml;/g, "ä")
                .replace(/&Auml;/g, "Ä")
                .replace(/&aring;/g, "å")
                .replace(/&Aring;/g, "Å");
}

Number.prototype.mod = function(n) {
    return ((this%n)+n)%n;
}
