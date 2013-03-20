import QtQuick 1.1

ListModel {
	id: list
	
	property list<ReRole> roles
    property string source: ""
    property string regExp: ""
	property string subQuery: "([A-Za-z]j)"
    property int status: XmlListModel.Loading
	
	Component.onCompleted: {
		var doc = new XMLHttpRequest();
        doc.onreadystatechange = function() {
            if (doc.readyState == XMLHttpRequest.DONE) {
                var re = new RegExp(regExp, "g");
				//console.log(doc.responseText);
				var l = doc.responseText.match(re);
				
				for (var x=0; x<l.length; x++) {
					var listItem = {};
					for (var i=0; i<roles.length; i++) {
						var subRe = new RegExp(roles[i].regExp);
                        listItem[roles[i].name] = roles[i].decode(subRe.exec(l[x])[1]);
					}
					list.append(listItem);
				}
                list.status = XmlListModel.Ready;
            }
        }
        doc.open("GET", source);
        doc.send();
	}
}
