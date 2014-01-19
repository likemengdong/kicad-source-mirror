/**
 * @file dialog_export_idf.cpp
 */

/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013  Cirilo Bernardo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <wxPcbStruct.h>
#include <appl_wxstruct.h>
#include <pcbnew.h>
#include <class_board.h>

// IDF export header generated by wxFormBuilder
#include <dialog_export_idf_base.h>

#define OPTKEY_IDF_THOU wxT( "IDFExportThou" )


bool Export_IDF3( BOARD *aPcb, const wxString & aFullFileName, double aUseThou );


class DIALOG_EXPORT_IDF3: public DIALOG_EXPORT_IDF3_BASE
{
private:
    PCB_EDIT_FRAME* m_parent;
    wxConfig* m_config;
    bool m_idfThouOpt;  // remember last preference for units in THOU

    void OnCancelClick( wxCommandEvent& event )
    {
        EndModal( wxID_CANCEL );
    }
    void OnOkClick( wxCommandEvent& event )
    {
        EndModal( wxID_OK );
    }

public:
    DIALOG_EXPORT_IDF3( PCB_EDIT_FRAME* parent ) :
            DIALOG_EXPORT_IDF3_BASE( parent )
    {
        m_parent = parent;
        m_config = wxGetApp().GetSettings();
        SetFocus();
        m_idfThouOpt = false;
        m_config->Read( OPTKEY_IDF_THOU, &m_idfThouOpt );
        m_chkThou->SetValue( m_idfThouOpt );

        GetSizer()->SetSizeHints( this );
        Centre();
    }
    
    ~DIALOG_EXPORT_IDF3()
    {
        m_idfThouOpt = m_chkThou->GetValue();
        m_config->Write( OPTKEY_IDF_THOU, m_idfThouOpt );
    }

    bool GetThouOption()
    {
        return m_chkThou->GetValue();
    }

    wxFilePickerCtrl* FilePicker()
    {
        return m_filePickerIDF;
    }
};


/**
 * Function OnExportIDF3
 * will export the current BOARD to IDF board and lib files.
 */
void PCB_EDIT_FRAME::ExportToIDF3( wxCommandEvent& event )
{
    wxFileName fn;

    // Build default file name
    fn = GetBoard()->GetFileName();
    fn.SetExt( wxT( "emn" ) );

    DIALOG_EXPORT_IDF3 dlg( this );
    dlg.FilePicker()->SetPath( fn.GetFullPath() );

    if ( dlg.ShowModal() != wxID_OK )
        return;

    bool thou = dlg.GetThouOption();

    wxBusyCursor dummy;

    wxString fullFilename = dlg.FilePicker()->GetPath();

    if ( !Export_IDF3( GetBoard(), fullFilename, thou ) )
    {
        wxString msg = _("Unable to create ") + fullFilename;
        wxMessageBox( msg );
        return;
    }
}