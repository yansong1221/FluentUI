#include "FluTools.h"

#include <QGuiApplication>
#include <QClipboard>
#include <QUuid>
#include <QCursor>
#include <QScreen>
#include <QColor>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QOpenGLContext>
#include <QCryptographicHash>
#include <QTextDocument>
#include <QQuickWindow>
#include <QDateTime>
#include <QSettings>

FluTools::FluTools(QObject *parent):QObject{parent}{

}

void FluTools::clipText(const QString& text){
    QGuiApplication::clipboard()->setText(text);
}

QString FluTools::uuid(){
    return QUuid::createUuid().toString().remove('-').remove('{').remove('}');
}

QString FluTools::readFile(const QString &fileName){
    QString content;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        content = stream.readAll();
    }
    return content;
}

bool FluTools::isMacos(){
#if defined(Q_OS_MACOS)
    return true;
#else
    return false;
#endif
}

bool FluTools::isLinux(){
#if defined(Q_OS_LINUX)
    return true;
#else
    return false;
#endif
}

bool FluTools::isWin(){
#if defined(Q_OS_WIN)
    return true;
#else
    return false;
#endif
}

int FluTools::qtMajor(){
    const QString qtVersion = QString::fromLatin1(qVersion());
    const QStringList versionParts = qtVersion.split('.');
    return versionParts[0].toInt();
}

int FluTools::qtMinor(){
    const QString qtVersion = QString::fromLatin1(qVersion());
    const QStringList versionParts = qtVersion.split('.');
    return versionParts[1].toInt();
}

void FluTools::setQuitOnLastWindowClosed(bool val){
    qApp->setQuitOnLastWindowClosed(val);
}

void FluTools::setOverrideCursor(Qt::CursorShape shape){
    qApp->setOverrideCursor(QCursor(shape));
}

void FluTools::restoreOverrideCursor(){
    qApp->restoreOverrideCursor();
}

void FluTools::deleteLater(QObject *p){
    if(p){
        p->deleteLater();
        p = nullptr;
    }
}

QString FluTools::toLocalPath(const QUrl& url){
    return url.toLocalFile();
}

QString FluTools::getFileNameByUrl(const QUrl& url){
    return QFileInfo(url.toLocalFile()).fileName();
}

QString FluTools::html2PlantText(const QString& html){
    QTextDocument textDocument;
    textDocument.setHtml(html);
    return textDocument.toPlainText();
}

QRect FluTools::getVirtualGeometry(){
    return qApp->primaryScreen()->virtualGeometry();
}

QString FluTools::getApplicationDirPath(){
    return qApp->applicationDirPath();
}

QUrl FluTools::getUrlByFilePath(const QString& path){
    return QUrl::fromLocalFile(path);
}

QColor FluTools::withOpacity(const QColor& color,qreal opacity){
    int alpha = qRound(opacity * 255) & 0xff;
    return QColor::fromRgba((alpha << 24) | (color.rgba() & 0xffffff));
}

QString FluTools::md5(QString text){
    return QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5).toHex();
}

QString FluTools::toBase64(QString text){
    return text.toUtf8().toBase64();
}

QString FluTools::fromBase64(QString text){
    return QByteArray::fromBase64(text.toUtf8());
}

bool FluTools::removeDir(QString dirPath){
    QDir qDir(dirPath);
    return qDir.removeRecursively();
}

bool FluTools::removeFile(QString filePath){
    QFile file(filePath);
    return file.remove();
}

QString FluTools::sha256(QString text){
    return QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha256).toHex();
}

void FluTools::showFileInFolder(QString path){
#if defined(Q_OS_WIN)
    QProcess::startDetached("explorer.exe", {"/select,", QDir::toNativeSeparators(path)});
#endif
#if defined(Q_OS_LINUX)
    QFileInfo fileInfo(path);
    auto process = "xdg-open";
    auto arguments = { fileInfo.absoluteDir().absolutePath() };
    QProcess::startDetached(process, arguments);
#endif
#if defined(Q_OS_MACOS)
    QProcess::execute("/usr/bin/osascript", {"-e", "tell application \"Finder\" to reveal POSIX file \"" + path + "\""});
    QProcess::execute("/usr/bin/osascript", {"-e", "tell application \"Finder\" to activate"});
#endif
}

bool FluTools::isSoftware(){
    return QQuickWindow::sceneGraphBackend() == "software";
}

QPoint FluTools::cursorPos(){
    return QCursor::pos();
}

qint64 FluTools::currentTimestamp(){
    return QDateTime::currentMSecsSinceEpoch();
}

QIcon FluTools::windowIcon(){
    return QGuiApplication::windowIcon();
}

int FluTools::cursorScreenIndex(){
    int screenIndex = 0;
    int screenCount = qApp->screens().count();
    if (screenCount > 1) {
        QPoint pos = QCursor::pos();
        for (int i = 0; i < screenCount; ++i) {
            if (qApp->screens().at(i)->geometry().contains(pos)) {
                screenIndex = i;
                break;
            }
        }
    }
    return screenIndex;
}

int FluTools::windowBuildNumber(){
#if defined(Q_OS_WIN)
    QSettings regKey {QString::fromUtf8("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), QSettings::NativeFormat};
    if (regKey.contains(QString::fromUtf8("CurrentBuildNumber"))) {
        auto buildNumber = regKey.value(QString::fromUtf8("CurrentBuildNumber")).toInt();
        return buildNumber;
    }
#endif
    return -1;
}

bool FluTools::isWindows11OrGreater(){
    static QVariant var;
    if(var.isNull()){
#if defined(Q_OS_WIN)
        auto buildNumber = windowBuildNumber();
        if(buildNumber>=22000){
            var = QVariant::fromValue(true);
            return true;
        }
#endif
        var = QVariant::fromValue(false);
        return  false;
    }else{
        return var.toBool();
    }
}

bool FluTools::isWindows10OrGreater(){
    static QVariant var;
    if(var.isNull()){
#if defined(Q_OS_WIN)
        auto buildNumber = windowBuildNumber();
        if(buildNumber>=10240){
            var = QVariant::fromValue(true);
            return true;
        }
#endif
        var = QVariant::fromValue(false);
        return  false;
    }else{
        return var.toBool();
    }
}

QRect FluTools::desktopAvailableGeometry(QQuickWindow* window){
    return window->screen()->availableGeometry();
}
