package org.kde.necessitas.origo;

import android.util.Log;
import java.io.File;
import android.content.Context;
import android.app.NativeActivity;

import android.content.Intent;
import android.net.Uri;
import android.webkit.MimeTypeMap;
import org.kde.necessitas.origo.QtActivity;

public class QJavaInterface /*extends NativeActivity*/ {
    public QJavaInterface() {
    }

    public static String getExternalFilesDir()
    {
        File file = QtActivity.getQtActivityInstance().getExternalFilesDir(null);
        return file.getPath();
    }

    public static void openURL(String url)
    {
        if (url.toLowerCase().startsWith("file:"))
        {
            String ext=url.substring(url.lastIndexOf(".")+1).toLowerCase();
            String type;
            if (ext.equals("flv")) {
                type = "video/flv";
            }
            else {
                MimeTypeMap mime = MimeTypeMap.getSingleton();
                type = mime.getMimeTypeFromExtension(ext);
            }

            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(Uri.parse(url),type);
            QtActivity.getQtActivityInstance().startActivity(intent);
        }
        else
        {
            Uri uri = Uri.parse(url);
            Intent intent = new Intent(Intent.ACTION_VIEW, uri);
            QtActivity.getQtActivityInstance().startActivity(intent);
        }
    }
}
