import QtQuick 1.1
import "jsonpath.js" as JSONPath

ListModel {
	id: list
	
    property string source: ""
    property variant query: ""
    property int status: XmlListModel.Loading
	
	Component.onCompleted: {
		try {
			var xhr = new XMLHttpRequest();
			xhr.onreadystatechange = function() {
				if (xhr.readyState == XMLHttpRequest.DONE) {
					try {
						var json = xhr.responseText;
						if ( json !== "" ) {
							var objectArray = JSON.parse(json);
							if ( query !== "" ) {
								var queries;
								if (typeof query == "string")
									queries = [ query ];
								else
									queries = query;
								for(var i=0; i<queries.length; i++) {
									var q = queries[i];
									var result = JSONPath.jsonPath(objectArray, q);
									for ( var key in result ) {
										var jo = result[key];
										list.append( jo );
									}
								}
							} else {
								for ( var key in objectArray ) {
									var jo = objectArray[key];
									list.append( jo );
								}
							}
						}
						list.status = XmlListModel.Ready;
					} catch(ex) {
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
