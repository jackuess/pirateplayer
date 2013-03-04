import QtQuick 1.1
import Components 1.0

BarredListView {
    anchors.fill: parent

    model: ListModel {
        ListElement {
            title: "SvT-play"
            url: "svt/initial.qml"
            logo: "http://www.svtplay.se/public/2012.54/images/svt-play-2x.png"
        }
        ListElement {
            title: "UR-play"
            url: "ur/a-z.qml"
            logo: "http://urplay.se/design/ur_play/images/urplay-logo.png"
        }
        ListElement {
            title: "TV3-play"
            url: "mtg/a-z.qml"//"imports/Components/AzListView.qml"
            logo: "http://images2.wikia.nocookie.net/__cb20091126144517/logopedia/images/c/c6/TV3_logo_1990.png"
            n: 3
        }
        ListElement {
            title: "TV4-play"
            url: "tv4/a-z.qml"
            logo: "http://images1.wikia.nocookie.net/__cb20100305140436/logopedia/images/3/31/TV4_Play.svg"
        }
        ListElement {
            title: "Kanal 5-play"
            url: "kanal5/a-z.qml"
            logo: "http://images2.wikia.nocookie.net/__cb20091126112949/logopedia/images/1/14/Kanal_5.svg"
        }
        ListElement {
            title: "TV6-play"
            url: "mtg/a-z.qml"//"imports/Components/AzListView.qml"
            logo: "http://www.tv6play.se/themes/play/images/tv6-logo.png"
            n: 6
        }
        ListElement {
            title: "TV8-play"
            url: "mtg/a-z.qml"//"imports/Components/AzListView.qml"
            logo: "http://www.viasat.se/sites-paytv/viasat.se/files/kanal8_play_0.png"
            n: 8
        }
//        ListElement {
//            title: "Kanal 9-play"
//            url: "kanal9/a-z.qml"
//            logo: "http://www.kanal9play.se/themes/kanal9/images/logo.png"
//        }
        ListElement {
            title: "Aftonbladet-TV"
            url: "aftonbladet/a-z.qml"
            logo: "http://www.tv14.net/wp-content/uploads/2010/10/Aftonbladet.jpg"
        }
    }

    delegate: ListDelegate {
        text: model.title
        imgSource: model.logo
        onClicked: {
            go( Qt.resolvedUrl(model.url),
               model.url == "mtg/a-z.qml" ? { n: model.n } : {},
               model.index);
        }
    }
}
