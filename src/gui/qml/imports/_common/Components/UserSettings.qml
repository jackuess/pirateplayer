import QtQuick 1.1

Item {
    property variant defaults: { "startDir": getHomeDir().match(/^(.*?)[\/\\]?$/),
                                 "playerCmd": (mac() ? applicationDirPath() + "/" : "") + "ffplay \"%0\"",
                                 "filenameTemplate": "%name%_%season%_%title%",
                                 "fontPointSize": getSetting("MainWindow/font_point_size") }

    property string startDir: getSetting("Location/start_dir", defaults.startDir)
    property string playerCmd: getSetting("Location/player_cmd", defaults.playerCmd)
    property string filenameTemplate: getSetting("Location/filename_template", defaults.filenameTemplate)
    property int fontPointSize: getSetting("MainWindow/font_point_size", defaults.fontPointSize)

    function save() {
        setSetting("Location/start_dir", startDir);
        setSetting("Location/player_cmd", playerCmd);
        setSetting("Location/filename_template", filenameTemplate);
        setSetting("MainWindow/font_point_size", fontPointSize)
    }

    function reset() {
        startDir = defaults.startDir;
        playerCmd = defaults.playerCmd;
        filenameTemplate = defaults.filenameTemplate;
        fontPointSize = defaults.fontPointSize;
    }
}
