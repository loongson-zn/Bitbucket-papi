/* 
* File:    papi_preset.c
* CVS:     
* Author:  Haihang You
*          you@cs.utk.edu
* Mods:    <your name here>
*          <your email address>
*/

#include "papi.h"
#include "papi_internal.h"
#include "papi_memory.h"

/* Defined in papi_data.c */
extern hwi_presets_t _papi_hwi_presets;

/* This routine copies values from a dense 'findem' array of events into the sparse
   global _papi_hwi_presets array, which is assumed to be empty at initialization. 
   Multiple dense arrays can be copied into the sparse array, allowing event overloading
   at run-time, or allowing a baseline table to be augmented by a model specific table
   at init time. This method supports adding new events; overriding existing events, or
   deleting deprecated events.
*/
int _papi_hwi_setup_all_presets(hwi_search_t * findem, hwi_dev_notes_t *notes, int substrate)
{
   int i, pnum, preset_index, did_something = 0;

   /* dense array of events is terminated with a 0 preset.
      don't do anything if NULL pointer. This allows just notes to be loaded.
      It's also good defensive programming. 
   */
   if (findem != NULL) {
      for (pnum = 0; (pnum < PAPI_MAX_PRESET_EVENTS) && (findem[pnum].event_code != 0);
         pnum++) {

         /* find the index for the event to be initialized */
         preset_index = (findem[pnum].event_code & PAPI_PRESET_AND_MASK);

         for(i=0;i<PAPI_MAX_COUNTER_TERMS;i++){
           if ( findem[pnum].data.native[i] != PAPI_NULL ){
              int blah = PAPI_SUBSTRATE_MASK(substrate);
              findem[pnum].data.native[i]|=blah;
           }
              //findem[pnum].data.native[i]|=PAPI_SUBSTRATE_MASK(substrate);
         }

         /* count and set the number of native terms in this event */
         for (i = 0; (i < PAPI_MAX_COUNTER_TERMS) && (findem[pnum].data.native[i] != PAPI_NULL); i++);
         _papi_hwi_presets.count[preset_index] = i;

         /* if the native event array is empty, free the data pointer.
            this allows existing events to be 'undefined' by overloading with nulls */
         if (i == 0) {
            if (_papi_hwi_presets.data[preset_index] != NULL) 
               papi_valid_free(_papi_hwi_presets.data[preset_index]);
         }
         /* otherwise malloc a data istructure for the sparse array and copy 
            the event data into it. Kevin assures me that the data won't 
            *actually* be duplicated unless it is modified */
        else {
            _papi_hwi_presets.data[preset_index] = papi_malloc(sizeof(hwi_preset_data_t));
            *_papi_hwi_presets.data[preset_index] = findem[pnum].data;
         }
         did_something++;
      }
   }
   /* optional dense array of event notes is terminated with a 0 preset */
   if (notes != NULL) {
      for (pnum = 0; (pnum < PAPI_MAX_PRESET_EVENTS) && (notes[pnum].event_code != 0);
         pnum++) {

         /* strdup the note string into the sparse preset data array */
         preset_index = (notes[pnum].event_code & PAPI_PRESET_AND_MASK);
         if (_papi_hwi_presets.dev_note[preset_index] != NULL)
            papi_free(_papi_hwi_presets.dev_note[preset_index]);
         _papi_hwi_presets.dev_note[preset_index] = papi_strdup(notes[pnum].dev_note);
      }
   }
   return (did_something ? 0 : PAPI_ESBSTR);
}

int _papi_hwi_cleanup_all_presets(void)
{
   int preset_index;

   for (preset_index = 0; preset_index < PAPI_MAX_PRESET_EVENTS; preset_index++) {
      /* free the names and descriptions if they were malloc'd by PAPI */
      papi_valid_free(_papi_hwi_presets.info[preset_index].symbol);
      _papi_hwi_presets.info[preset_index].symbol = NULL;
      papi_valid_free(_papi_hwi_presets.info[preset_index].long_descr);
      _papi_hwi_presets.info[preset_index].long_descr = NULL;
      papi_valid_free(_papi_hwi_presets.info[preset_index].short_descr);
      _papi_hwi_presets.info[preset_index].short_descr = NULL;

      /* free the data and or note string if they exist */
      if (_papi_hwi_presets.data[preset_index] != NULL) {
         papi_free(_papi_hwi_presets.data[preset_index]);
         _papi_hwi_presets.data[preset_index] = NULL;
      }
      if (_papi_hwi_presets.dev_note[preset_index] != NULL) {
         _papi_hwi_presets.dev_note[preset_index] = NULL;
      }
   }
   return (PAPI_OK);
}

/* The following code is proof of principle for reading preset events from an
   xml file. It has been tested and works for pentium3. It relys on the expat
   library and is invoked by adding
   XMLFLAG		= -DXML
   to the Makefile. It is presently hardcoded to look for "./papi_events.xml"
*/
#ifdef XML

#define BUFFSIZE 8192
#define SPARSE_BEGIN 0
#define SPARSE_EVENT_SEARCH 1
#define SPARSE_EVENT 2
#define SPARSE_DESC 3
#define ARCH_SEARCH 4
#define DENSE_EVENT_SEARCH 5
#define DENSE_NATIVE_SEARCH 6
#define DENSE_NATIVE_DESC 7
#define FINISHED 8

char buffer[BUFFSIZE], *xml_arch;
int location=SPARSE_BEGIN, sparse_index=0, native_index, error=0;

/* The function below, _xml_start(), is a hook into expat's XML
 * parser.  _xml_start() defines how the parser handles the
 * opening tags in PAPI's XML file.  This function can be understood
 * more easily if you follow along with its logic while looking at
 * papi_events.xml.  The location variable is a global telling us
 * where we are in the XML file.  Have we found our architecture's
 * events yet?  Are we looking at an event definition?...etc.
 */
static void _xml_start(void *data, const char *el, const char **attr) {
   int native_encoding;

   if(location==SPARSE_BEGIN && !strcmp("papistdevents", el)) {
      location=SPARSE_EVENT_SEARCH;
   }
   else if(location==SPARSE_EVENT_SEARCH && !strcmp("papievent", el)) {
      _papi_hwi_presets.info[sparse_index].symbol = papi_strdup(attr[1]);
//      strcpy(_papi_hwi_presets.info[sparse_index].symbol, attr[1]);
      location=SPARSE_EVENT;
   }
   else if(location==SPARSE_EVENT && !strcmp("desc", el)) {
      location=SPARSE_DESC;
   }
   else if(location==ARCH_SEARCH && !strcmp("availevents", el) &&
           !strcmp(xml_arch, attr[1])) {
      location=DENSE_EVENT_SEARCH;
   }
   else if(location==DENSE_EVENT_SEARCH && !strcmp("papievent", el)) {
      if(!strcmp("PAPI_NULL", attr[1])) {
         location=FINISHED;
         return;
      }
      else if(PAPI_event_name_to_code((char *)attr[1], &sparse_index) != PAPI_OK) {
         PAPIERROR("Improper Preset name given in XML file for %s.", attr[1]);
         error=1;
      }
      sparse_index &= PAPI_PRESET_AND_MASK;

      /* allocate and initialize data space for this event */
      papi_valid_free(_papi_hwi_presets.data[sparse_index]);
      _papi_hwi_presets.data[sparse_index] = papi_malloc(sizeof(hwi_preset_data_t));
      native_index=0;
      _papi_hwi_presets.data[sparse_index]->native[native_index]=PAPI_NULL;
      _papi_hwi_presets.data[sparse_index]->operation[0]='\0';

        
     if(attr[2]) {                      /* derived event */
         _papi_hwi_presets.data[sparse_index]->derived = _papi_hwi_derived_type((char *)attr[3]);
         /* where does DERIVED POSTSCRIPT get encoded?? */
         if (_papi_hwi_presets.data[sparse_index]->derived == -1) {
            PAPIERROR("No derived type match for %s in Preset XML file.", attr[3]);
            error=1;
         }

         if(attr[5]) {
            _papi_hwi_presets.count[sparse_index] = atoi(attr[5]);
         }
         else {
            PAPIERROR("No count given for %s in Preset XML file.", attr[1]);
            error=1;
         }
      }
      else {
         _papi_hwi_presets.data[sparse_index]->derived=NOT_DERIVED;
         _papi_hwi_presets.count[sparse_index] = 1;
      }
      location=DENSE_NATIVE_SEARCH;
   }
   else if(location==DENSE_NATIVE_SEARCH && !strcmp("native", el)) {
      location=DENSE_NATIVE_DESC;
   }
   else if(location==DENSE_NATIVE_DESC && !strcmp("event", el)) {
      if(_papi_hwi_native_name_to_code(attr[1], &native_encoding)!=PAPI_OK) {
         printf("Improper Native name given in XML file for %s\n", attr[1]);
         PAPIERROR("Improper Native name given in XML file for %s\n", attr[1]);
         error=1;
      }
      _papi_hwi_presets.data[sparse_index]->native[native_index]=native_encoding;
      native_index++;
      _papi_hwi_presets.data[sparse_index]->native[native_index]=PAPI_NULL;
   }
   else if(location && location!=ARCH_SEARCH && location!=FINISHED) {
      PAPIERROR("Poorly-formed Preset XML document.");
      error=1;
   }
}

/* The function below, _xml_end(), is a hook into expat's XML
 * parser.  _xml_end() defines how the parser handles the
 * end tags in PAPI's XML file.
 */
static void _xml_end(void *data, const char *el) {
   int i;

   if(location==SPARSE_EVENT_SEARCH && !strcmp("papistdevents", el)) {
      for(i=sparse_index; i<PAPI_MAX_PRESET_EVENTS; i++) {
         _papi_hwi_presets.info[i].symbol = NULL;
         _papi_hwi_presets.info[i].long_descr = NULL;
         _papi_hwi_presets.info[i].short_descr = NULL;
      }
      location=ARCH_SEARCH;
   }
   else if(location==DENSE_NATIVE_DESC && !strcmp("native", el)) {
      location=DENSE_EVENT_SEARCH;
   }
   else if(location==DENSE_EVENT_SEARCH && !strcmp("availevents", el)) {
      location=FINISHED;
   }
}

/* The function below, _xml_content(), is a hook into expat's XML
 * parser.  _xml_content() defines how the parser handles the
 * text between tags in PAPI's XML file.  The information between
 * tags is usally text for event descriptions.
 */
static void _xml_content(void *data, const char *el, const int len) {
   int i;
   if(location==SPARSE_DESC) {
      _papi_hwi_presets.info[sparse_index].long_descr = papi_malloc(len+1);
      for(i=0; i<len; i++) _papi_hwi_presets.info[sparse_index].long_descr[i]=el[i];
      _papi_hwi_presets.info[sparse_index].long_descr[len]='\0';
      /* the XML data currently doesn't contain a short description */
      _papi_hwi_presets.info[sparse_index].short_descr = NULL;
      sparse_index++;
      _papi_hwi_presets.data[sparse_index] = NULL;
      location=SPARSE_EVENT_SEARCH;
   }
}

int _xml_papi_hwi_setup_all_presets(char *arch, hwi_dev_notes_t *notes) {
   int done = 0;
   FILE *fp = fopen("./papi_events.xml", "r");
   XML_Parser p = XML_ParserCreate(NULL);

   if(!p) {
      PAPIERROR("Couldn't allocate memory for XML parser.");
      return(PAPI_ESYS);
   }
   XML_SetElementHandler(p, _xml_start, _xml_end);
   XML_SetCharacterDataHandler(p, _xml_content);
   if(fp==NULL) {
      PAPIERROR("Error opening Preset XML file.");
      return(PAPI_ESYS);
   }

   xml_arch = arch;

   do {
      int len;
      void *buffer = XML_GetBuffer(p, BUFFSIZE);

      if(buffer==NULL) {
         PAPIERROR("Couldn't allocate memory for XML buffer.");
         return(PAPI_ESYS);
      }
      len = fread(buffer, 1, BUFFSIZE, fp);
      if(ferror(fp)) {
         PAPIERROR("XML read error.");
         return(PAPI_ESYS);
      }
      done = feof(fp);
      if(!XML_ParseBuffer(p, len, len == 0)) {
         PAPIERROR("Parse error at line %d:\n%s\n",
         XML_GetCurrentLineNumber(p),
         XML_ErrorString(XML_GetErrorCode(p)));
         return(PAPI_ESYS);
      }
      if(error) return(PAPI_ESYS);
   } while(!done);
   XML_ParserFree(p);
   fclose(fp);
   return(PAPI_OK);
}
#endif
