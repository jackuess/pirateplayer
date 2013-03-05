import QtQuick 1.1

Item {
    property variant defaults: { "startDir": getHomeDir(),
                                 "playerCmd": (mac() ? applicationDirPath() + "/" : "") + "ffplay %0",
                                 "filenameTemplate": "%name%_-_%title%",
                                 "pirateplaySeBase": "http://pirateplay.se",
                                 "fontPointSize": getSetting("MainWindow/font_point_size") }

    property string startDir: getSetting("Location/start_dir", defaults.startDir)
    property string playerCmd: getSetting("Location/player_cmd", defaults.playerCmd)
    property string filenameTemplate: getSetting("Location/filename_template", defaults.filenameTemplate)
    property string pirateplaySeBase: getSetting("Debug/pirateplay_se_base", defaults.pirateplaySeBase)
    property int fontPointSize: getSetting("MainWindow/font_point_size", defaults.fontPointSize)

    function save() {
        setSetting("Location/start_dir", startDir);
        setSetting("Location/player_cmd", playerCmd);
        setSetting("Location/filename_template", filenameTemplate);
        setSetting("Debug/pirateplay_se_base", pirateplaySeBase);
        setSetting("MainWindow/font_point_size", fontPointSize)
    }

    function reset() {
        startDir = defaults.startDir;
        playerCmd = defaults.playerCmd;
        filenameTemplate = defaults.filenameTemplate;
        pirateplaySeBase = defaults.pirateplaySeBase;
        fontPointSize = defaults.fontPointSize;
    }
}
