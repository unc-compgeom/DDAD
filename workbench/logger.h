/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-08-16
 */

#ifndef RC_LOGGER_H
#define RC_LOGGER_H

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

#endif // RC_LOGGER_H
