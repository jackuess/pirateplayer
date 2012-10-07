#include <QDebug>
#include <QIODevice>

#include "time.h"

#include "archiveextractor.h"

Q_DECLARE_METATYPE(QIODevice *)

ArchiveExtractor::ArchiveExtractor(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QIODevice *>("QIODevice *");
    qRegisterMetaType<QByteArray *>("QByteArray *");
}

void ArchiveExtractor::extractToDir(const char *archivePath, const char *outDir) {
    QMetaObject::invokeMethod(this, "extractToDirLocal", Qt::QueuedConnection, Q_ARG(const QString, archivePath), Q_ARG(const QString, outDir));
}

void ArchiveExtractor::extractToDir(QByteArray *data, const QString &outDir) {
    QMetaObject::invokeMethod(this, "extractToDirQBA", Qt::QueuedConnection, Q_ARG(QByteArray *, data), Q_ARG(const QString &, outDir));
}

void ArchiveExtractor::extractToDir(QIODevice *data, const QString &outDir) {
    QMetaObject::invokeMethod(this, "extractToDirQIO", Qt::QueuedConnection, Q_ARG(QIODevice *, data), Q_ARG(const QString &, outDir));
}

void ArchiveExtractor::extractToDir(QNetworkReply *data, const QString &outDir) {
    QMetaObject::invokeMethod(this, "extractToDirQIO", Qt::QueuedConnection, Q_ARG(QIODevice *, data), Q_ARG(const QString &, outDir));
}

void ArchiveExtractor::extractToDirLocal(const QString archivePath, const QString &outDir) {
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;
    QString newPath;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_compression_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_filename(a, qPrintable(archivePath), 10240)))
        exit(1);
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r != ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            exit(1);

        const char* path = archive_entry_pathname( entry );
        newPath = QString("%1/%2").arg(outDir, path);
        archive_entry_set_pathname( entry, qPrintable(newPath) );

        r = archive_write_header(ext, entry);
        if (r != ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
            copyData(a, ext);
            if (r != ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                exit(1);
        }
        r = archive_write_finish_entry(ext);
        if (r != ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            exit(1);
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);

    emit finished();
}

void ArchiveExtractor::extractToDirQIO(QIODevice *data, const QString &outDir) {
    QByteArray ba = data->readAll();
    extractToDirQBA(&ba, outDir);
}

void ArchiveExtractor::extractToDirQBA(QByteArray *data, const QString &outDir) {
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;
    QString newPath;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_compression_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_memory(a, data->data(), data->size())))
        exit(1);
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r != ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            exit(1);

        const char* path = archive_entry_pathname( entry );
        newPath = QString("%1/%2").arg(outDir, path);
        archive_entry_set_pathname( entry, qPrintable(newPath) );

        r = archive_write_header(ext, entry);
        if (r != ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
            copyData(a, ext);
            if (r != ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                exit(1);
        }
        r = archive_write_finish_entry(ext);
        if (r != ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            exit(1);
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);

    emit finished();
}

int ArchiveExtractor::copyData(archive *ar, archive *aw) {
    int r;
    const void *buff;
    size_t size;
    off64_t offset;

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r != ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r != ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(aw));
            return (r);
        }
    }
}
