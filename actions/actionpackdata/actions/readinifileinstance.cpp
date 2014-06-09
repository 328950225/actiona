/*
	Actionaz
    Copyright (C) 2008-2014 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "readinifileinstance.h"
#include "stringlistpair.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace Actions
{
	ActionTools::StringListPair ReadIniFileInstance::modes = qMakePair(
            QStringList() << "singleParameter" << "wholeFile",
            QStringList() << QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read a single parameter") << QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read the entire file"));

	void ReadIniFileInstance::startExecution()
	{
		bool ok = true;

		QString filename = evaluateString(ok, "file");
		QString variable = evaluateVariable(ok, "variable");
		Mode mode = evaluateListElement<Mode>(ok, modes, "mode");

		if(!ok)
			return;

        if(filename.isEmpty())
        {
            emit executionException(UnableToReadFileException, tr("Unable to read the file"));
            return;
        }

        boost::property_tree::ptree tree;

        try
        {
            boost::property_tree::ini_parser::read_ini(filename.toStdString(), tree);
        }
        catch(const std::runtime_error &e)
        {
            setCurrentParameter("filename");
            emit executionException(UnableToReadFileException, tr("Unable to read the file"));
            return;
        }

        if(mode == WholeFile)
		{
            QHash<QString, QString> fileContent;

            for(const auto &section: tree)
            {
                for(const auto &parameter: section.second)
                {
                    fileContent[QString::fromStdString(section.first + "/" + parameter.first)] = QString::fromStdString(parameter.second.get_value<std::string>());
                }
            }

            setArrayKeyValue(variable, fileContent);
        }
        else
        {
            QString section = evaluateString(ok, "section");
            QString parameter = evaluateString(ok, "parameter");

            if(!ok)
                return;

            auto sectionNode = tree.get_child_optional(section.toStdString());
            if(!sectionNode)
            {
                setCurrentParameter("section");
                emit executionException(UnableToFindSectionException, tr("Unable to find the section named \"%1\"").arg(section));
                return;
            }

            try
            {
                setVariable(variable, QString::fromStdString((*sectionNode).get<std::string>(parameter.toStdString())));
            }
            catch(const std::runtime_error &e)
            {
                setCurrentParameter("parameter");
                emit executionException(UnableToFindSectionException, tr("Unable to find the parameter named \"%1\"").arg(parameter));
                return;
            }
        }

		emit executionEnded();
	}
}
