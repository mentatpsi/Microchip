/************************************************************************
* Copyright (c) 2009,  Microchip Technology Inc.
*
* Microchip licenses this software to you solely for use with Microchip
* products.  The software is owned by Microchip and its licensors, and
* is protected under applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
* MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
* EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
* OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
* TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
* OR OTHER SIMILAR COSTS.
*
* To the fullest extent allowed by law, Microchip and its licensors
* liability shall not exceed the amount of fees, if any, that you
* have paid directly to Microchip to use this software.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*
* Author        Date        Comment
*************************************************************************
* E. Schlunder  2009/04/29  Initial code.
************************************************************************/

#include "Settings.h"
#include "ui_Settings.h"

#include <QtWidgets/QMessageBox>

Settings::Settings(QWidget *parent) : QDialog(parent), m_ui(new Ui::Settings)
{
    m_ui->setupUi(this);

    alreadyWarnedConfigBitWrite = false;
}

Settings::~Settings()
{
    delete m_ui;
}

void Settings::enableEepromBox(bool Eeprom)
{
    m_ui->EepromCheckBox->setEnabled(Eeprom);
    hasEeprom = Eeprom;
}

void Settings::setWriteFlash(bool value)
{
    writeFlash = value;
    m_ui->FlashProgramMemorycheckBox->setChecked(value);
}

void Settings::setWriteEeprom(bool value)
{
    writeEeprom = value && hasEeprom;
    m_ui->EepromCheckBox->setChecked(value && hasEeprom);
}

void Settings::setWriteConfig(bool value)
{
    writeConfig = value && hasConfig;
    bool warnedFlag = alreadyWarnedConfigBitWrite;
    alreadyWarnedConfigBitWrite = true;
    m_ui->ConfigBitsCheckBox->setChecked(value && hasConfig);
    alreadyWarnedConfigBitWrite = warnedFlag;
}

void Settings::changeEvent(QEvent *e)
{
    switch (e->type())
    {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void Settings::on_buttonBox_accepted()
{
    writeFlash = m_ui->FlashProgramMemorycheckBox->isChecked();
    writeConfig = m_ui->ConfigBitsCheckBox->isChecked();
    writeEeprom = m_ui->EepromCheckBox->isChecked();
}

void Settings::on_ConfigBitsCheckBox_toggled(bool checked)
{
    if((alreadyWarnedConfigBitWrite == false) && checked)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Warning!");

        msgBox.setText("Writing Config Bits is not a safe bootloader operation.\n" \
                       "\n" \
                       "The device and bootloader may stop functioning if the new config bit settings are\nnot compatible with USB operation.\n" \
                       "When the bootloader becomes inoperable, restoring the device will not be possible\n" \
                       "without dedicated external chip programming tools.\n" \
                       "\n" \
                       "Are you sure you wish to enable the \"Write Config Bits\" option?");

        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int result = msgBox.exec();
        if(result != QMessageBox::Ok)
        {
            m_ui->ConfigBitsCheckBox->setChecked(false);
            return;
        }

        alreadyWarnedConfigBitWrite = true;
    }
}
