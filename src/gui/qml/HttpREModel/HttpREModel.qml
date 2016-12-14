import QtQuick 1.1

ListModel {
	id: list
	
	property list<ReRole> roles
    property string source: ""
    property string regExp: ""
	property string subQuery: "([A-Za-z]j)"
    property int status: XmlListModel.Loading
	
	Component.onCompleted: {
		try {
			var xhr = new XMLHttpRequest();
	        xhr.onreadystatechange = function() {
	        	try {
		            if (xhr.readyState == XMLHttpRequest.DONE) {
		                var re = new RegExp(regExp, "g");
						//list.append({text:xhr.responseText});
						var l = xhr.responseText.match(re);
						
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
				} catch(ex) {
					list.append({text:ex.toString()});
					list.status = XmlListModel.Error;
				}
	        }
	        xhr.open("GET", source);
	        xhr.send();
		} catch(ex) {
			list.append({text:ex.toString()});
			list.status = XmlListModel.Error;
		}
	}
}
