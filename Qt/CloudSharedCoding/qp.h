
#ifndef QP_H
#define QP_H

#include <QPlainTextEdit>



class QP : public QPlainTextEdit
{
public:
    QP(QPlainTextEdit*qp);
    QTextBlock firstVBlock();
    QRectF blockBGeometry(QTextBlock block);
    QPointF contentOset();
    QRectF blockBRect(QTextBlock block);
    QPlainTextEdit*qp;
};

#endif // QP_H
