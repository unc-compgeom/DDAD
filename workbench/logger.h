/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WB_LOGGER_H
#define WB_LOGGER_H

#include "common.h"

BEGIN_NAMESPACE(DDAD)

/*
class RLOG_DECL RCLogger : public rlog::RLogNode {
public:
    RCLogger();
    RCLogger(QTextEdit* console);
    virtual ~RCLogger();

    void set_console(QTextEdit* console);
    void subscribeTo(rlog::RLogNode *node);

protected:
    virtual void publish(const rlog::RLogData &data);

    RCLogger(const RCLogger &);
    RCLogger& operator=(const RCLogger&);

private:
    QTextEdit* console_;
};
*/
END_NAMESPACE(DDAD)

#endif // WB_LOGGER_H
