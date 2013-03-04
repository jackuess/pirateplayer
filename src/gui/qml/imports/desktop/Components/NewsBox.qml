import QtQuick 1.1
import QtDesktop 0.1

GroupBox {
    id: newsBox

    property int currentIndex: 0

    function getCurrent(role) {
        return newsFeed.status == XmlListModel.Ready ? newsFeed.get(newsBox.currentIndex)[role] : ""
    }

    height: 120
    title: "Nyheter"

    XmlListModel {
        id: newsFeed

        source: "http://api.twitter.com/1/statuses/user_timeline.xml?screen_name=pirateplay_se&exclude_replies=true&count=200&trim_user=true"
        query: "//status"

        XmlRole {
            name: "text"
            query: "text/string()"
        }
        XmlRole {
            name: "pubDate"
            query: "created_at/string()"
        }
        XmlRole {
            name: "id"
            query: "id/string()"
        }
    }

    Column {
        anchors { margins: 5; left: parent.left; right: twitterButtons.left; top: parent.top; bottom: parent.bottom }
        spacing: 10

        Text {
            function formatTweet(t) {
                return t.replace(/(https?:\/\/\S+)/gm, "<a href=\"$1\">$1</a>");
            }

            width: parent.width
            text: formatTweet(newsBox.getCurrent("text"))
            wrapMode: Text.WordWrap
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            width: parent.width
            color: "gray"
            text: "Publicerad " + formatTweetTime(newsBox.getCurrent("pubDate")) + " via <a href=\"http://twitter.com/pirateplay_se/statuses/" + newsBox.getCurrent("id") + "\">Twitter</a>"
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
    Row {
        anchors.margins: 5
        anchors.right: parent.right
        id: twitterButtons
        Button {
            text: "Föregående"
            enabled: newsFeed.count > 0 && newsBox.currentIndex > 0
            onClicked: newsBox.currentIndex--
        }
        Button {
            text: "Nästa"
            enabled: newsFeed.count > 0 && newsBox.currentIndex < newsFeed.count-1
            onClicked: newsBox.currentIndex++
        }
    }
}
