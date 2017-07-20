/// \file
/// \brief Starts an interactive root session in a web browser
///
/// This script starts a http server session, with a canvas and an output
/// log and spawning a web browser, showing how to create a very simple
/// interactive root session in a web browser
///
/// N.B. This script has to be executed with ACLiC!!!!!
///
/// \macro_code
///
/// \author Bertrand Bellenot
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
   THTreeViewer(TTree *tree = 0, );
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
   gSystem->Exec("google-chrome --app=\"http://localhost:8097\" --window-position=200,100 --window-size=\"1150,650\" 2>/dev/null &");
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

/**
 * \macro tree_viewer
 *
 * \brief Starts an interactive root session in a web browser
 *
 * This script starts a http server session, with a canvas and an output
 * log and spawning a web browser, showing how to create a very simple
 * interactive root session in a web browser
 *
 */
void alja()
{
/*
   TFile *file = TFile::Open("hsimple.root");
   if (!file) return;
   TTree *tree = 0;
   file->GetObject("ntuple", tree);
   if (!tree) return;
   THTreeViewer *viewer = new THTreeViewer(tree);
*/
}

