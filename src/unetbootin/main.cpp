#include "unetbootin.h"

#ifdef Q_OS_WIN32

void configsysUndo(QString uninstPathL)
{
	if (!QFile::copy(QDir::toNativeSeparators(QString("%1unetbtin/config.sys").arg(uninstPathL)), QDir::toNativeSeparators(QString("%1config.sys").arg(uninstPathL))))
	{
		QFile::remove(QDir::toNativeSeparators(QString("%1config.sys").arg(uninstPathL)));
		QFile::copy(QDir::toNativeSeparators(QString("%1unetbtin/config.sys").arg(uninstPathL)), QDir::toNativeSeparators(QString("%1config.sys").arg(uninstPathL)));
	}
	SetFileAttributesA(QDir::toNativeSeparators(QString("%1config.sys").arg(uninstPathL)).toLocal8Bit(), FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_ARCHIVE);
}

void bootiniUndo(QString uninstPathL)
{
	if (!QFile::copy(QDir::toNativeSeparators(QString("%1unetbtin/boot.ini").arg(uninstPathL)), QDir::toNativeSeparators(QString("%1boot.ini").arg(uninstPathL))))
		{
			QFile::remove(QDir::toNativeSeparators(QString("%1boot.ini").arg(uninstPathL)));
			QFile::copy(QDir::toNativeSeparators(QString("%1unetbtin/boot.ini").arg(uninstPathL)), QDir::toNativeSeparators(QString("%1boot.ini").arg(uninstPathL)));
		}
	SetFileAttributesW(LPWSTR(QDir::toNativeSeparators(QString("%1boot.ini").arg(uninstPathL)).utf16()), FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_ARCHIVE);
}

void vistabcdUndo(QString uninstPathL)
{
	QSettings vdtustor("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\UNetbootin", QSettings::NativeFormat);
	QVariant warch64varL(QVariant::Bool);
	warch64varL = vdtustor.value("WArch64");
	bool warch64L = warch64varL.value<bool>();
	unetbootin uexecRunL;
	if (warch64L)
	{
		uexecRunL.callexternapp(QDir::toNativeSeparators(QString("%1unetbtin/emtxfile.exe").arg(uninstPathL)), QDir::toNativeSeparators(QString("%1unetbtin/vbcdundo.bat runas").arg(uninstPathL)));
	}
	else
	{
		uexecRunL.callexternapp(QDir::toNativeSeparators(QString("%1unetbtin/vbcdundo.bat").arg(uninstPathL)), "");
	}
	vdtustor.remove("WArch64");
}

#endif

void clearOutDir(QString pDirToDel)
{
	QDir dirToDel(pDirToDel);
	QStringList rmfileList = dirToDel.entryList(QDir::Files);
	for (int i = 0; i < rmfileList.size(); ++i)
	{
		QFile::setPermissions(QDir::toNativeSeparators(QString("%1/%2").arg(pDirToDel).arg(rmfileList.at(i))), QFile::WriteOther);
		QFile::remove(QDir::toNativeSeparators(QString("%1/%2").arg(pDirToDel).arg(rmfileList.at(i))));
	}
	dirToDel.rmdir(pDirToDel);
}

void ubnUninst()
{
	QSettings autostrt("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", QSettings::NativeFormat);
 	if (autostrt.contains("UNetbootin Uninstaller"))
 	{
 		autostrt.remove("UNetbootin Uninstaller");
	}
	QSettings chkinstL("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\UNetbootin", QSettings::NativeFormat);
	QVariant uninstvar(QVariant::String);
	uninstvar = chkinstL.value("Location");
	QString uninstPath = uninstvar.value<QString>();
	#ifdef Q_OS_WIN32
	if (QSysInfo::WindowsVersion == QSysInfo::WV_32s || QSysInfo::WindowsVersion == QSysInfo::WV_95 || QSysInfo::WindowsVersion == QSysInfo::WV_98 || QSysInfo::WindowsVersion == QSysInfo::WV_Me)
	{
		configsysUndo(uninstPath);
	}
	else if (QSysInfo::WindowsVersion == QSysInfo::WV_NT || QSysInfo::WindowsVersion == QSysInfo::WV_2000 || QSysInfo::WindowsVersion == QSysInfo::WV_XP || QSysInfo::WindowsVersion == QSysInfo::WV_2003 )
	{
		bootiniUndo(uninstPath);
	}
	else if (QSysInfo::WindowsVersion == QSysInfo::WV_VISTA)
	{
		vistabcdUndo(uninstPath);
	}
	else
	{
		configsysUndo(uninstPath);
		bootiniUndo(uninstPath);
		vistabcdUndo(uninstPath);
	}
	#endif
	clearOutDir(QDir::toNativeSeparators(QString("%1unetbtin").arg(uninstPath)));
	QFile::remove(QDir::toNativeSeparators(QString("%1ubnldr.exe").arg(uninstPath)));
	QFile::remove(QDir::toNativeSeparators(QString("%1ubnldr").arg(uninstPath)));
	QFile::remove(QDir::toNativeSeparators(QString("%1ubnldr.mbr").arg(uninstPath)));
	chkinstL.clear();
	QMessageBox finmsgb;
	finmsgb.setIcon(QMessageBox::Information);
	finmsgb.setWindowTitle(QObject::tr("Uninstallation Complete"));
	finmsgb.setText(QObject::tr("UNetbootin has been uninstalled."));
 	finmsgb.setStandardButtons(QMessageBox::Ok);
 	switch (finmsgb.exec())
 	{
 		case QMessageBox::Ok:
 			break;
		default:
			break;
 	}
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv, true);
	QTranslator translator;
	translator.load(QDir::toNativeSeparators(QString("%1/unetbootin_%2").arg(app.applicationDirPath()).arg(QLocale::system().name())));
	app.installTranslator(&translator);
    QSettings chkinst("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\UNetbootin", QSettings::NativeFormat);
	if (chkinst.contains("Location"))
	{
		QMessageBox uninstmsgb;
		uninstmsgb.setIcon(QMessageBox::Information);
		uninstmsgb.setWindowTitle(QObject::tr("UNetbootin Uninstaller"));
		uninstmsgb.setText(QObject::tr("UNetbootin is currently installed. Remove the existing version?"));
 		uninstmsgb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
 		switch (uninstmsgb.exec())
 		{
 			case QMessageBox::Ok:
 			{
 				ubnUninst();
			}
			case QMessageBox::Cancel:
				break;
	 		default:
				break;
 		}
		return 0;
	}
	unetbootin unetbootin;
	unetbootin.appDir = QDir::toNativeSeparators(QString("%1/").arg(app.applicationDirPath()));
	unetbootin.appLoc = app.applicationFilePath();
    unetbootin.show();
    return app.exec();
}
