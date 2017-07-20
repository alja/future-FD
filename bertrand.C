/// \file
/// \ingroup tutorial_eve
/// Complex example showing ALICE VSD visualization.
///
///  alice_vsd.C - a simple event-display for ALICE
///
///  Only standard ROOT is used to process the ALICE VSD files.
///
///  No ALICE code is needed -- the VSD file is exported from AliRoot into
///  VSD format -- see TEveVSDStructs.h and TEveVSD.h.
///
///  A simple geometry of 10KB, extracted from the full TGeo-geometry, is
///  used to outline the central detectors of ALICE.
///
///  All files are access from the web by using the "CACHEREAD" option.
///
/// \image html eve_alice_vsd.png
/// \macro_code
///
/// \author Matevz Tadel


#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveVSD.h>
#include <TEveVSDStructs.h>

#include <TEveTrack.h>
//#include <TEveTrackList.h>
#include <TEveTrackPropagator.h>
#include <TEveGeoShape.h>

#include <TGTab.h>
#include <TGButton.h>

#include <TFile.h>
#include <TKey.h>
#include <TSystem.h>
#include <TPRegexp.h>

#include <THttpServer.h>
#include <stdio.h>

#include "TApplication.h"
#include "TCanvas.h"
#include "TNamed.h"
#include "TList.h"
#include "TObjString.h"
#include "TSystem.h"
#include "THttpServer.h"
#include "TDatime.h"
#include "TClass.h"
#include "TFile.h"
#include "TTree.h"
#include "TList.h"
#include "TMap.h"
#include "Riostream.h"
#include <sstream>
#include <TEveGeoShape.h>
#include <TEveManager.h>

// Include componets -- compile time link :)

#include "MultiView.C"
// #include "bertrand_hist_include.C"
MultiView* gMultiView = 0;


/**
 * \class TOutputLog
 *
 * \brief Simple output log
 *
 * This class is simply reading a text file used as output redirection, and 
 * used to send the content of the file in a TObjString to the JavaScript code 
 * running in a web browser.
 *
 */
class TOutputLog : public TNamed {
protected:
   TObjString *fLogString;
public:
   TOutputLog(const char* name = "log") : TNamed(name, "log messages"), 
               fLogString(0) { fLogString = new TObjString(); }
   virtual ~TOutputLog() { }

   TObjString *ReadOutputLog(const char *fname = "output_log.txt") {
      TString msg;
      std::ifstream instr(fname);
      msg.ReadFile(instr);
      fLogString->SetString(msg.Data());
      instr.close();
      gSystem->Unlink(fname);
      return fLogString;
   }
   ClassDef(TOutputLog, 1); // simple output log
};


/**
 * \class THTreeViewer
 *
 * \brief Simple Html Tree Viewer
 *
 * This class is simply the interface between the C++ Tree and the Html interface 
 * of a Tree Viewer running in a web browser.
 *
 */
class THTreeViewer : public TNamed {
protected:
   TList fTreeList;
   TMap fCanvases;
   TTree *fTree;
   TOutputLog *fOutputLog;
   THttpServer *fServer;
   TCanvas *fCurrCanvas;
public:
   THTreeViewer(TTree *tree = 0, THttpServer* server = 0) ;
   virtual ~THTreeViewer();

     
   void  DrawSelection(const char *varexp, const char *selection, const char *options, Long64_t nentries=1000000000, Long64_t firstentry=0);

   ClassDef(THTreeViewer, 1); // simple tree viewer interface
};

THTreeViewer::THTreeViewer(TTree *tree, THttpServer* server) : TNamed("viewer", "simple tree viewer")
{

   fTree = tree;
   fCurrCanvas = 0;
   if (fTree) fTreeList.Add(fTree);
   gSystem->Unlink("output_log.txt");
   // create an instance of our output logger
   fOutputLog = new TOutputLog();
   // start the http server
   fServer = server;
   // set our own custom page as default one
   //fServer->SetDefaultPage("./alja.html");
   // register the output log
   fServer->Register("/", fOutputLog);
   // while the server runs in read-only mode, we should allow methods execution
   fServer->Restrict("/fOutputLog", "allow_method=ReadOutputLog");
   // register simple processline command
   fServer->RegisterCommand("/ProcessLine", "%arg1% >& output_log.txt");
   fServer->Register("/", this);
   // start a web browser
   gSystem->Exec("google-chrome --app=\"http://localhost:5281\" --window-position=200,100 --window-size=\"1150,650\" 2>/dev/null &");
}

THTreeViewer::~THTreeViewer()
{
   gSystem->Unlink("output_log.txt");
   fServer = new THttpServer("http:8097?top=ROOT;readwrite");
   fServer->Unregister(this);
   fServer->Unregister(fOutputLog);
   delete fOutputLog;
   fOutputLog = 0;
   delete fServer;
   fServer = 0;
}

// TTree::Draw(const char *varexp, const char *selection, Option_t *option="", Long64_t nentries=kMaxEntries, Long64_t firstentry=0)
void THTreeViewer::DrawSelection(const char *varexp, const char *selection, const char *options, Long64_t nentries, Long64_t firstentry)
{
   printf("AMT THTreeViewer::DrawSelection()\n");
   if (!fTree) return;
   TCanvas *c1 = new TCanvas("c1", "c1");
   fCanvases.Add(new TObjString("c1"), c1);
   fCurrCanvas = c1;
   fServer->Register("/", c1);
   if (fTree) fTree->Draw(varexp, selection, options, nentries, firstentry);
}

class TVSDReader
{
public:
   // ----------------------------------------------------------
   // File / Event Data
   // ----------------------------------------------------------

   TFile      *fFile;
   TDirectory *fDirectory;

   TObjArray  *fEvDirKeys;

   TEveVSD    *fVSD;

   Int_t       fMaxEv, fCurEv;

   // ----------------------------------------------------------
   // Event visualization structures
   // ----------------------------------------------------------

   TEveTrackList *fTrackList;
   TEvePointSet  *fITSClusters;
   TEvePointSet  *fTPCClusters;
   TEvePointSet  *fTRDClusters;
   TEvePointSet  *fTOFClusters;

public:
   TVSDReader(const char* file_name) :
      fFile(0), fDirectory(0), fEvDirKeys(0),
      fVSD(0),

      fMaxEv(-1), fCurEv(-1),

      fTrackList(0),
      fITSClusters(0), fTPCClusters(0), fTRDClusters(0), fTOFClusters(0)
   {
      fFile = TFile::Open(file_name);
      if (!fFile)
      {
         Error("VSD_Reader", "Can not open file '%s' ... terminating.",
               file_name);
         gSystem->Exit(1);
      }

      fEvDirKeys = new TObjArray;
      TPMERegexp name_re("Event\\d+");
      TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
      while (lnk)
      {
         if (name_re.Match(lnk->GetObject()->GetName()))
         {
            fEvDirKeys->Add(lnk->GetObject());
         }
         lnk = lnk->Next();
      }

      fMaxEv = fEvDirKeys->GetEntriesFast();
      if (fMaxEv == 0)
      {
         Error("VSD_Reader", "No events to show ... terminating.");
         gSystem->Exit(1);
      }

      fVSD = new TEveVSD;
   }

   virtual ~TVSDReader()
   {
      // Destructor.

      DropEvent();

      delete fVSD;
      delete fEvDirKeys;

      fFile->Close();
      delete fFile;
   }

   void AttachEvent()
   {
      // Attach event data from current directory.

      fVSD->LoadTrees();
      fVSD->SetBranchAddresses();
   }

   void DropEvent()
   {
      // Drup currently held event data, release current directory.

      // Drop old visualization structures.

      gEve->GetViewers()->DeleteAnnotations();
      gEve->GetCurrentEvent()->DestroyElements();

      // Drop old event-data.

      fVSD->DeleteTrees();
      delete fDirectory;
      fDirectory = 0;
   }

   //---------------------------------------------------------------------------
   // Event navigation
   //---------------------------------------------------------------------------

   void NextEvent()
   {
      GotoEvent(fCurEv + 1);
   }

   void PrevEvent()
   {
      GotoEvent(fCurEv - 1);
   }

   Bool_t GotoEvent(Int_t ev)
   {
      if (ev < 0 || ev >= fMaxEv)
      {
         Warning("GotoEvent", "Invalid event id %d.", ev);
         return kFALSE;
      }

      DropEvent();

      // Connect to new event-data.

      fCurEv = ev;
      fDirectory = (TDirectory*) ((TKey*) fEvDirKeys->At(fCurEv))->ReadObj();
      fVSD->SetDirectory(fDirectory);

      AttachEvent();

      // Load event data into visualization structures.

      LoadClusters(fITSClusters, "ITS", 0);
      LoadClusters(fTPCClusters, "TPC", 1);
      LoadClusters(fTRDClusters, "TRD", 2);
      LoadClusters(fTOFClusters, "TOF", 3);

      LoadEsdTracks();

      // Fill projected views.

      TEveElement* top = gEve->GetCurrentEvent();

      gMultiView->DestroyEventRPhi();
      gMultiView->ImportEventRPhi(top);

      gMultiView->DestroyEventRhoZ();
      gMultiView->ImportEventRhoZ(top);

      gEve->Redraw3D(kFALSE, kTRUE);

      return kTRUE;
   }


   //---------------------------------------------------------------------------
   // Cluster loading
   //---------------------------------------------------------------------------

   void LoadClusters(TEvePointSet*& ps, const TString& det_name, Int_t det_id)
   {
      if (ps == 0)
      {
         ps = new TEvePointSet(det_name);
         ps->SetMainColor((Color_t)(det_id + 2));
         ps->SetMarkerSize(0.5);
         ps->SetMarkerStyle(2);
         ps->IncDenyDestroy();
      }
      else
      {
         ps->Reset();
      }

      TEvePointSelector ss(fVSD->fTreeC, ps, "fV.fX:fV.fY:fV.fZ",
                           TString::Format("fDetId==%d", det_id));
      ss.Select();
      ps->SetTitle(TString::Format("N=%d", ps->Size()));

      gEve->AddElement(ps);
   }


   //---------------------------------------------------------------------------
   // Track loading
   //---------------------------------------------------------------------------

   enum ESDTrackFlags
   {
      kITSin=0x0001,kITSout=0x0002,kITSrefit=0x0004,kITSpid=0x0008,
      kTPCin=0x0010,kTPCout=0x0020,kTPCrefit=0x0040,kTPCpid=0x0080,
      kTRDin=0x0100,kTRDout=0x0200,kTRDrefit=0x0400,kTRDpid=0x0800,
      kTOFin=0x1000,kTOFout=0x2000,kTOFrefit=0x4000,kTOFpid=0x8000,
      kHMPIDpid=0x20000,
      kEMCALmatch=0x40000,
      kTRDbackup=0x80000,
      kTRDStop=0x20000000,
      kESDpid=0x40000000,
      kTIME=0x80000000
   };

   Bool_t trackIsOn(TEveTrack* t, Int_t mask)
   {
      // Check is track-flag specified by mask are set.

      return (t->GetStatus() & mask) > 0;
   }

   void LoadEsdTracks()
   {
      // Read reconstructed tracks from current event.

      if (fTrackList == 0)
      {
         fTrackList = new TEveTrackList("ESD Tracks");
         fTrackList->SetMainColor(6);
         fTrackList->SetMarkerColor(kYellow);
         fTrackList->SetMarkerStyle(4);
         fTrackList->SetMarkerSize(0.5);

         fTrackList->IncDenyDestroy();
      }
      else
      {
         fTrackList->DestroyElements();
      }

      TEveTrackPropagator* trkProp = fTrackList->GetPropagator();
      // !!!! Need to store field on file !!!!
      // Can store TEveMagField ?
      trkProp->SetMagField(0.5);
      trkProp->SetStepper(TEveTrackPropagator::kRungeKutta);

      Int_t nTracks = fVSD->fTreeR->GetEntries();
      for (Int_t n = 0; n < nTracks; ++n)
      {
         fVSD->fTreeR->GetEntry(n);

         TEveTrack* track = new TEveTrack(&fVSD->fR, trkProp);
         track->SetName(Form("ESD Track %d", fVSD->fR.fIndex));
         track->SetStdTitle();
         track->SetAttLineAttMarker(fTrackList);
         fTrackList->AddElement(track);
      }

      fTrackList->MakeTracks();

      gEve->AddElement(fTrackList);
   }

   ClassDef(TVSDReader, 0);
};


TVSDReader* gVSDReader = 0;

TEveGeoShapeExtract* gVSDGEOM = 0; // temporary solution, one need geometry which can be streamed

class TableColumn : public TNamed {
public:
   TableColumn() {
   }
   std::vector<std::string> fTest;

   ClassDef(TableColumn, 1);
  
};

class TEveHttp : public TNamed {

   protected:
      TList* fEvent;
      TList* fTable;

   public:
      TEveHttp() : TNamed(), fEvent(0) {}
      TEveHttp(const char* name, const char* title) : TNamed(name, title), fEvent(0), fTable(0) {}

      TEveGeoShapeExtract* GetGeometry() {
         // just return geometry, which can be streamed to the client
         return gVSDGEOM;
      }

      TList* GetEvent() {
         // extract from TEveEventManager objectsm which can be streamed
         // Temporary solution, works with TEvePointSet and TEveTrackList
         TEveEventManager* ev = gEve->GetCurrentEvent();
         if (!ev) return 0;

         if (!fEvent) { fEvent = new TList(); fEvent->SetOwner(kFALSE); }
                 else { while (fEvent->GetSize()>0) fEvent->RemoveLast(); }

         for (TEveElement::List_i iter = ev->BeginChildren(); iter != ev->EndChildren(); ++iter) {
            TEveElement* chld = *iter;
            TEvePointSet* pnts = dynamic_cast<TEvePointSet*> (chld);
            // add TEvePointSet, they can be streamed
            if (pnts) { fEvent->Add(pnts); continue; }

            TEveTrackList* tracks = dynamic_cast<TEveTrackList*> (chld);
            if (tracks) {
               // printf("TEveTrackList %s \n", tracks->GetElementName());
               // track list is not streamable, add separate tracks
               for (TEveElement::List_i iter2 = tracks->BeginChildren(); iter2 != tracks->EndChildren(); ++iter2) {
                  TEveTrack* track = dynamic_cast<TEveTrack*> (*iter2);
                  if (track) fEvent->Add(track);
               }
               continue;
            }
         }

         return fEvent;
      }

      int NextEvent() {
         printf("Extract next event\n");
         gVSDReader->NextEvent();
         return 1;
      }

      int PrevEvent() {
         printf("Extract previous event\n");
         gVSDReader->PrevEvent();
         return 1;
      }


   TList* GetTable() {
      printf("Calling GetTable\n");
         
      TEveEventManager* ev = gEve->GetCurrentEvent();
      if (!ev) return 0;


      if (!fTable) { fTable = new TList(); fTable->SetOwner(kFALSE); }
      else { while (fTable->GetSize()>0) fTable->RemoveLast(); }

      

      //   TableColumn* name = new TableColumn(); name->SetName("name");
      TableColumn* pt   = new TableColumn(); pt->SetName("pt");
      TableColumn* eta  = new TableColumn(); eta->SetName("eta");
      TableColumn* phi  = new TableColumn(); phi->SetName("phi");

      char buff[1000];
      for (TEveElement::List_i iter = ev->BeginChildren(); iter != ev->EndChildren(); ++iter) {
         TEveTrackList* tracks = dynamic_cast<TEveTrackList*> (*iter);
         if (tracks) {
            for (TEveElement::List_i iter2 = tracks->BeginChildren(); iter2 != tracks->EndChildren(); ++iter2) {
               TEveTrack* track = dynamic_cast<TEveTrack*> (*iter2);
               if (track) {
               printf("tttst\n");
               sprintf(buff, "%.2f",  track->GetMomentum().Perp());
               pt->fTest.push_back(buff);
               
               sprintf(buff, "%.2f",  track->GetMomentum().Eta());
               eta->fTest.push_back(buff);
               
               sprintf(buff, "%.2f",  track->GetMomentum().Phi());
               phi->fTest.push_back(buff);
               
               }
            }
         }
         
         }

      fTable->Add(pt);
      fTable->Add(eta);
      fTable->Add(phi);

      return fTable;
   }
                

   ClassDef(TEveHttp, 1);
};


void start_http() {
   THttpServer* serv = new THttpServer("http:5281;rw");
   serv->AddLocation("genkiDir/", "/var/www/html/alja/");
   
   TEveHttp* sniff = new TEveHttp("eve","access to EVE structures");

   serv->Register("/", sniff);

   serv->SetDefaultPage("bertrand.html");

// from original alja.C

   TFile *file = TFile::Open("hsimple.root");
   if (!file) return;
   TTree *tree = 0;
   file->GetObject("ntuple", tree);
   if (!tree) return;

   THTreeViewer *viewer = new THTreeViewer(tree, serv);
}


// Forward declaration.
void make_gui();

//______________________________________________________________________________
void bertrand(const char* vsd_file_name=
               "http://mtadel.home.cern.ch/mtadel/root/AliVSD.root")
{
   // Main function, initializes the application.
   //
   // 1. Load the auto-generated library holding ESD classes and
   //    ESD dictionaries.
   // 2. Open ESD data-files.
   // 3. Load cartoon geometry.
   // 4. Spawn simple GUI.
   // 5. Load first event.

   TFile::SetCacheFileDir(".");

   TEveVSD::DisableTObjectStreamersForVSDStruct();

   gVSDReader = new TVSDReader(vsd_file_name);

   TEveManager::Create();

   TEveGeoShape *gentle_geom = 0;

   { // Simple geometry
      TFile* geom =
      TFile::Open("http://mtadel.home.cern.ch/mtadel/root/alice_mini_geom.root",
                  "CACHEREAD");
      if (!geom)
         return;
      TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
      gVSDGEOM = gse;
      gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
      geom->Close();
      delete geom;
      gEve->AddGlobalElement(gentle_geom);

      printf("GENTLE %s\n", gentle_geom->GetName());
   }


   // Standard multi-view
   //=====================

   gMultiView = new MultiView;
   gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);

   gMultiView->SetDepth(-10);
   gMultiView->ImportGeomRPhi(gentle_geom);
   gMultiView->ImportGeomRhoZ(gentle_geom);
   gMultiView->SetDepth(0);


   // Final stuff
   //=============

   gEve->GetViewers()->SwitchColorSet();
   gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);

   gEve->GetBrowser()->GetTabRight()->SetTab(1);

   make_gui();

   gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event"));

   gVSDReader->GotoEvent(0);

   gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.


   start_http();
}


//______________________________________________________________________________
void make_gui()
{
   // Create minimal GUI for event navigation.

   TEveBrowser* browser = gEve->GetBrowser();
   browser->StartEmbedding(TRootBrowser::kLeft);

   TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
   frmMain->SetWindowName("XX GUI");
   frmMain->SetCleanup(kDeepCleanup);

   TGHorizontalFrame* hf = new TGHorizontalFrame(frmMain);
   {
      TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")));
      TGPictureButton* b = 0;

      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoBack.gif"));
      hf->AddFrame(b);
      b->Connect("Clicked()", "TVSDReader", gVSDReader, "PrevEvent()");

      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoForward.gif"));
      hf->AddFrame(b);
      b->Connect("Clicked()", "TVSDReader", gVSDReader, "NextEvent()");
   }
   frmMain->AddFrame(hf);

   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();

   browser->StopEmbedding();
   browser->SetTabTitle("Event Control", 0);
}

// #endif
