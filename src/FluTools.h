#ifndef FLUTOOLS_H
#define FLUTOOLS_H

#include <QObject>
#include <QFile>
#include <QColor>
#include <QtQml/qqml.h>
#include <QQuickWindow>
#include "singleton.h"

/**
 * @brief The FluTools class
 */
class FluTools : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(FluTools)
    QML_SINGLETON
private:
    explicit FluTools(QObject *parent = nullptr);
public:
    SINGLETON(FluTools)
    static FluTools *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine){return getInstance();}
    Q_INVOKABLE int qtMajor();
    Q_INVOKABLE int qtMinor();
    Q_INVOKABLE bool isMacos();
    Q_INVOKABLE bool isLinux();
    Q_INVOKABLE bool isWin();
    Q_INVOKABLE void clipText(const QString& text);
    Q_INVOKABLE QString uuid();
    Q_INVOKABLE QString readFile(const QString& fileName);
    Q_INVOKABLE void setQuitOnLastWindowClosed(bool val);
    Q_INVOKABLE void setOverrideCursor(Qt::CursorShape shape);
    Q_INVOKABLE void restoreOverrideCursor();
    Q_INVOKABLE QString html2PlantText(const QString& html);
    Q_INVOKABLE QString toLocalPath(const QUrl& url);
    Q_INVOKABLE void deleteLater(QObject *p);
    Q_INVOKABLE QString getFileNameByUrl(const QUrl& url);
    Q_INVOKABLE QRect getVirtualGeometry();
    Q_INVOKABLE QString getApplicationDirPath();
    Q_INVOKABLE QUrl getUrlByFilePath(const QString& path);
    Q_INVOKABLE QColor withOpacity(const QColor&,qreal alpha);
    Q_INVOKABLE QString md5(QString text);
    Q_INVOKABLE QString sha256(QString text);
    Q_INVOKABLE QString toBase64(QString text);
    Q_INVOKABLE QString fromBase64(QString text);
    Q_INVOKABLE bool removeDir(QString dirPath);
    Q_INVOKABLE bool removeFile(QString filePath);
    Q_INVOKABLE void showFileInFolder(QString path);
    Q_INVOKABLE bool isSoftware();
    Q_INVOKABLE qint64 currentTimestamp();
    Q_INVOKABLE QPoint cursorPos();
    Q_INVOKABLE QIcon windowIcon();
    Q_INVOKABLE int cursorScreenIndex();
    Q_INVOKABLE int windowBuildNumber();
    Q_INVOKABLE bool isWindows11OrGreater();
    Q_INVOKABLE bool isWindows10OrGreater();
    Q_INVOKABLE QRect desktopAvailableGeometry(QQuickWindow* window);
};

#endif // FLUTOOLS_H
