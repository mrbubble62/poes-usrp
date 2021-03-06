/*
    HRPT-Decoder, a software for processing NOAA-POES high resolution weather satellite images.
    Copyright (C) 2009 Free Software Foundation, Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Email: <postmaster@poes-weather.com>
    Web: <http://www.poes-weather.com>
*/
//---------------------------------------------------------------------------
#include "imagewidget.h"
#include "ui_imagewidget.h"

#include "mainwindow.h"
#include "block.h"
#include "plist.h"

#define F_NO_EVENTS 256
//---------------------------------------------------------------------------
ImageWidget::ImageWidget(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::ImageWidget)
{
    m_ui->setupUi(this);

    mw = (MainWindow *)parent;

    flags = 0;
}

//---------------------------------------------------------------------------
ImageWidget::~ImageWidget()
{
    delete m_ui;
}

//---------------------------------------------------------------------------
void ImageWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//---------------------------------------------------------------------------
int ImageWidget::getChannel(void)
{
 int channel;

   channel = m_ui->channelSpinBox->value();

  return (channel - 1);
}

//---------------------------------------------------------------------------
void ImageWidget::setMaxChannels(int channels)
{
    m_ui->channelSpinBox->setMaximum(channels);
}

//---------------------------------------------------------------------------
void ImageWidget::setFrames(QString format, long int frames)
{
 QString str;

  m_ui->typeLabel->setText(format);

  str.sprintf("%ld Frames", frames);
  m_ui->framesLabel->setText(str);
}

//---------------------------------------------------------------------------
bool ImageWidget::isNorthbound(void)
{
  return m_ui->NorthboundCb->isChecked();
}

//---------------------------------------------------------------------------
void ImageWidget::setProperties(bool northbound)
{
    TBlock *block = mw->getBlock();
    int i;

    flags |= F_NO_EVENTS;

    m_ui->NorthboundCb->setChecked(northbound);
    block->setNorthBound(isNorthbound());
    m_ui->channelSpinBox->setMaximum(block->getNumChannels());
    m_ui->channelSpinBox->setValue(1);
    block->setImageChannel(1);

    if(block->satprop->rgblist->Count == 0)
        block->satprop->add_rgb_defaults();

    i = m_ui->enhanceCb->currentIndex();

    m_ui->enhanceCb->clear();
    m_ui->enhanceCb->addItems(block->getImageTypes());

    if(i < 0 || i >= m_ui->enhanceCb->count())
        i = 0;

    m_ui->enhanceCb->setCurrentIndex(i);
    block->setImageType(i);
    block->checkSatProps();

    flags &= ~F_NO_EVENTS;
}

//---------------------------------------------------------------------------
int ImageWidget::getImageType(void)
{ 
    return m_ui->enhanceCb->currentIndex();
}

//---------------------------------------------------------------------------
int ImageWidget::getImageIndex(void)
{
    return m_ui->enhanceCb->currentIndex();
}

//---------------------------------------------------------------------------
void ImageWidget::on_channelSpinBox_valueChanged(int value)
{
    if(flags & F_NO_EVENTS)
        return;

    TBlock *b = mw->getBlock();

    if(b->getImageType() == Channel_ImageType) {
        b->setImageChannel(value);
        mw->renderImage();
    }
}

//---------------------------------------------------------------------------
void ImageWidget::on_NorthboundCb_clicked()
{
    if(flags & F_NO_EVENTS)
        return;

    TBlock *b = mw->getBlock();

    b->setNorthBound(isNorthbound());
    mw->renderImage();
}

//---------------------------------------------------------------------------
void ImageWidget::on_enhanceCb_currentIndexChanged(int index)
{
    if(flags & F_NO_EVENTS)
        return;

    TBlock *b = mw->getBlock();

    b->setImageType(index);
    mw->renderImage();
}

//---------------------------------------------------------------------------
