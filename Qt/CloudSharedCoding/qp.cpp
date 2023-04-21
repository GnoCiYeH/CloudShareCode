
#include "qp.h"
#include "qtextobject.h"

QP::QP(QPlainTextEdit*qp):QPlainTextEdit(qp)
{
    this->qp=qp;
}

QTextBlock QP::firstVBlock(){
    return firstVisibleBlock();
}

QRectF QP::blockBGeometry(QTextBlock block){
    return blockBoundingGeometry(block);
}

QPointF QP::contentOset(){
    return contentOffset();
}

QRectF QP::blockBRect(QTextBlock block){
    return blockBoundingRect(block);
}
