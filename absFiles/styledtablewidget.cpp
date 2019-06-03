#include "styledtablewidget.h"

StyledTableWidget::StyledTableWidget(QWidget *parent) : QTableWidget(parent)
{
    this->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->setEditTriggers(EditTrigger::NoEditTriggers);
    this->setShowGrid(false);
    this->horizontalHeader()->setMinimumSectionSize(40);
    this->horizontalHeader()->setStretchLastSection(true);
    this->setHorizontalScrollMode(ScrollMode::ScrollPerPixel);
    this->setVerticalScrollMode(ScrollMode::ScrollPerItem);
    this->setFocusPolicy(Qt::NoFocus); //去除选中虚线框
}
