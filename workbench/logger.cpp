/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 08/16/2013
 */

// Workbench
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
