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

// DDAD
#include "common.h"
#include "logger.h"

/*
// logger
#include "RLogPublisher.h"
#include "RLogChannel.h"

using namespace rlog;

namespace DDAD {

RCLogger::RCLogger() :
    RLogNode() {}

RCLogger::RCLogger(QTextEdit* console) :
    RLogNode(),
    console_(console) {}

RCLogger::~RCLogger() {}

void RCLogger::subscribeTo(RLogNode *node) {
    addPublisher(node);
    node->isInterested(this, true);
}

void RCLogger::publish(const RLogData &data) {
    char timeStamp[32];
    time_t errTime = data.time;
    tm currentTime = *localtime(&errTime);
    sprintf(timeStamp, "%02i:%02i:%02i ",
            currentTime.tm_hour,
            currentTime.tm_min,
            currentTime.tm_sec);

    std::stringstream ss;

    ss << timeStamp;
    ss << '[' << data.publisher->channel->name() << "] ";
    if(data.publisher->channel->name() != "info") {
        ss << "(" << data.publisher->fileName << ':'
           << data.publisher->lineNum << ") ";
    }
    ss << data.msg;

    std::string out = ss.str();
    console_->append(QString::fromStdString(out));
}

void RCLogger::set_console(QTextEdit *console) {
    console_ = console;
}

}
*/
