//
// Copyright (c) 2015-2017, Chaos Software Ltd
//
// V-Ray For Houdini IPR
//
// ACCESSIBLE SOURCE CODE WITHOUT DISTRIBUTION OF MODIFICATION LICENSE
//
// Full license text: https://github.com/ChaosGroup/vray-for-houdini/blob/master/LICENSE
//

#include "vfh_ipr_server.h"
#include "vfh_log.h"

#include <QApplication>
#include <QtCore>
#include <QProcessEnvironment>
#include <QDir>

int main(int argc, char ** argv)
{
	VRayForHoudini::Log::Logger::startLogging();
	std::shared_ptr<void> _stopLoggingAtExit = std::shared_ptr<void>(nullptr, [](void*) {
		VRayForHoudini::Log::Logger::stopLogging();
	});

	// We need to load "platform" plugins for windows, so we use one from houdini installation
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	QString hfsPath = env.value("HFS", "");
	if (hfsPath.isEmpty()) {
		VRayForHoudini::Log::getLog().error("Environment variable \"HFS\" missiing! IPR not started!");
		return 1;
	}
	QCoreApplication::addLibraryPath(hfsPath + "/bin/Qt_plugins/");

	QApplication app(argc, argv);
	Server server;

	return app.exec();
}
