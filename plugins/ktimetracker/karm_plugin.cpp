/*
    This file is part of Kontact.

    Copyright (c) 2004 Tobias Koenig <tokoe@kde.org>
    adapted for karm 2005 by Thorsten Staerk <kde@staerk.de>
    karm renamed to ktimetracker 2007-2008

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/

#include <kgenericfactory.h>
#include <kparts/componentfactory.h>
#include <kicon.h>
#include <kactioncollection.h>
#include "core.h"
#include "plugin.h"

#include <karm_part.h>
#include "karm_plugin.h"
#include "ktimetrackerinterface.h"

using namespace KTimeTracker;

typedef KGenericFactory<KarmPlugin, Kontact::Core> KarmPluginFactory;
K_EXPORT_COMPONENT_FACTORY( kontact_karm,
                            KarmPluginFactory( "kontact_karm" ) )

KarmPlugin::KarmPlugin( Kontact::Core *core, const QStringList& )
  : Kontact::Plugin( core, core, "KArm" ), mInterface( 0 )
{
  setComponentData( KarmPluginFactory::componentData() );

  KAction *action  = new KAction(KIcon("ktimetracker"), i18n("New Task"), this);
  actionCollection()->addAction("new_task", action );
  action->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_W));
  connect(action, SIGNAL(triggered(bool)), SLOT( newTask() ));
  insertNewAction(action);
}

KarmPlugin::~KarmPlugin()
{
}

OrgKdeKtimetrackerKtimetrackerInterface *KarmPlugin::interface()
{
  if ( !mInterface ) {
    part();
  }
  Q_ASSERT( mInterface );
  return mInterface;
}

QString KarmPlugin::tipFile() const
{
  // TODO: tips file
  //QString file = KStandardDirs::locate("data", "karm/tips");
  QString file;
  return file;
}

KParts::ReadOnlyPart* KarmPlugin::createPart()
{
  KParts::ReadOnlyPart * part = loadPart();

  connect( part, SIGNAL(showPart()), this, SLOT(showPart()) );
  mInterface = new OrgKdeKtimetrackerKtimetrackerInterface( "org.kde.ktimetracker", "/KTimeTracker", QDBusConnection::sessionBus() );

  return part;
}

QStringList KarmPlugin::configModules() const
{
  QStringList modules;
  modules << "karmplugin.desktop";
  return modules;
}

void KarmPlugin::showPart()
{
  core()->selectPlugin(this);
}

void KarmPlugin::newTask()
{
  kDebug() <<"Entering newTask";
  mInterface->addTask("New Task");
}

#include "karm_plugin.moc"
