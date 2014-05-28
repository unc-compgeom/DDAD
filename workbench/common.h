/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-08-13
 */

#ifndef RC_COMMON_H
#define RC_COMMON_H

#define BEGIN_NAMESPACE(x) namespace x {
#define END_NAMESPACE(x) }

// Qt
#include <QtGui>
#include <QtOpenGL>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QTextEdit>
#include <QThread>
#include <QMatrix4x4>
#include <QVector3D>
#include <QHash>
#include <QMap>
#include <QVector>
#include <QTimer>

// system
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <array>
#include <memory>
#include <deque>
#include <stack>
#include <map>
#include <limits>
#include <cstdlib>

// logging
#include "../logging/easylogging++.h"
/*
#include "rlog.h"
#include "rloglocation.h"
#include "RLogChannel.h"
#include "RLogTime.h"
#include "StdioNode.h"
*/

#endif // RC_COMMON_H
