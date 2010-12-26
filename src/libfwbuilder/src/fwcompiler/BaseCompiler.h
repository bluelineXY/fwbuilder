/* 

                          Firewall Builder

                 Copyright (C) 2009 NetCitadel, LLC

  Author:  Vadim Kurland     vadim@fwbuilder.org

  $Id$

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef __BASE_COMPILER_HH__
#define __BASE_COMPILER_HH__

#include <fwbuilder/libfwbuilder-config.h>
#include "fwbuilder/FWException.h"
#include "fwbuilder/Rule.h"

#include <sstream>

namespace fwcompiler {

    class FWCompilerException : public libfwbuilder::FWException {
	libfwbuilder::Rule *rule;
	public:
	FWCompilerException(libfwbuilder::Rule *r,const std::string &err);
	libfwbuilder::Rule *getRule() const { return rule; }
    };

    class BaseCompiler {

        std::string level_macro;
	std::stringstream errors_buffer;
        bool test_mode;
        bool embedded_mode;

        /**
         * assembles standard error message using format similar to
         * the error message format of gcc. Useful to prepare errors
         * and warnings before calling error() or warning() methods
         */
        std::string stdErrorMessage(libfwbuilder::FWObject *fw,
                                    libfwbuilder::FWObject *ruleset,
                                    libfwbuilder::FWObject *rule,
                                    const std::string &errstr);

        std::string setLevel(const std::string &level, const std::string &errstr);

        void printError(const std::string &errstr);

        void message(const std::string &level,
                     libfwbuilder::FWObject *fw,
                     libfwbuilder::FWObject *ruleset,
                     libfwbuilder::FWObject *rule,
                     const std::string &errstr);

public:

        void setTestMode() { test_mode = true; }
        bool inTestMode() { return test_mode; }

        void setEmbeddedMode() { embedded_mode = true; }
        bool inEmbeddedMode() { return embedded_mode; }

        /**
         * prints error message and aborts the program. If compiler is
         * in testing mode (flag test_mode==true), then just prints
         * the error message and returns.
         */
	virtual void abort(const std::string &errstr) throw(libfwbuilder::FWException);
	virtual void abort(libfwbuilder::FWObject *fw,
                           libfwbuilder::FWObject *ruleset,
                           libfwbuilder::FWObject *rule,
                           const std::string &errstr) throw(libfwbuilder::FWException);

        /**
         * prints an error message and returns
         */
	virtual void error(const std::string &warnstr);
	virtual void error(libfwbuilder::FWObject *fw,
                           libfwbuilder::FWObject *ruleset,
                           libfwbuilder::FWObject *rule,
                           const std::string &warnstr);

        /**
         * prints warning message
         */
	virtual void warning(const std::string &warnstr);
	virtual void warning(libfwbuilder::FWObject *fw,
                             libfwbuilder::FWObject *ruleset,
                             libfwbuilder::FWObject *rule,
                             const std::string &warnstr);

        /**
         * prints info message. These are only printed to stdout if compiler
         * is not in embedded mode. In embedded mode info messages are ignored.
         */
	virtual void info(const std::string &warnstr);
        
	virtual ~BaseCompiler() {};

	BaseCompiler() {test_mode = false; embedded_mode = false; level_macro = "%LEVEL%";};

	std::string getErrors(const std::string &comment_sep);
	bool haveErrorsAndWarnings();
        void clearErrors();

        /**
         * fills a list of strings with regular expressions that match
         * error messages
         */
        static void errorRegExp(std::list<std::string> *err_regexp);

        /**
         * fills a list of strings with regular expressions that match
         * warning messages
         */
        static void warningRegExp(std::list<std::string> *warn_regexp);

    };
}

#endif