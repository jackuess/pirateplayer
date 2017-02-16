import QtQuick 1.1
import "jsonpath.js" as JSONPath

ListModel {
	id: list
	
    property string source: ""
    property string regExp: ""
    property variant query: ""
    property int status: XmlListModel.Loading
	
	Component.onCompleted: {
		try {
			var xhr = new XMLHttpRequest();
			xhr.onreadystatechange = function() {
				if (xhr.readyState == XMLHttpRequest.DONE) {
					try {
						var json;
						if (regExp != "") {
			                var re = new RegExp(regExp);
							var m = xhr.responseText.match(re);
							if (m.length == 0)
								json = "";
							else if (m.length == 1)
								json = m[0];
							else
								json = m[1];
						} else {
							json = xhr.responseText;
						}
						if (json !== "") {
							var objectArray = JSON.parse(json);
							if (query != "") {
								var queries;
								if (typeof query == "string")
									queries = [ query ];
								else
									queries = query;
								for(var i=0; i<queries.length; i++) {
									var q = queries[i];
									var result = JSONPath.jsonPath(objectArray, q);
									for(var key in result) {
										var jo = result[key];
										list.append(jo);
									}
								}
							} else {
								for(var key in objectArray) {
									var jo = objectArray[key];
									list.append(jo);
								}
							}
						}
						list.status = XmlListModel.Ready;
					} catch(ex) {
						list.append({name: ex.toString()});
						list.status = XmlListModel.Error;
					}
				}
			}
			xhr.open("GET", source);
			xhr.send();
		} catch(ex) {
			list.status = XmlListModel.Error;
		}
	}
}
