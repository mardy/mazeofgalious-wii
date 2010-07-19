#ifdef _WIN32
#include "windows.h"
#else
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wii_utils.h"

#ifdef _WII
#include <unistd.h>
#endif

#include "debug.h"

#define NAME_MAX	256

/* Paths: */ 

extern char **g_paths;
extern int n_g_paths,act_g_path;
extern char *g_path;
extern char **s_paths;
extern int n_s_paths,act_s_path;
extern char *s_path;


#ifdef _WIN32
void init_paths(void)
{    
	if (g_paths==NULL) {

		WIN32_FIND_DATA finfo;
		HANDLE h;
		int i,j;
		char **tmp_g_paths; 

		n_g_paths=0;
		h=FindFirstFile("graphics\\*.*",&finfo);
		if (h!=INVALID_HANDLE_VALUE) {
			if (strcmp(finfo.cFileName,".")!=0 &&
				strcmp(finfo.cFileName,"..")!=0) {
				i=0;
				n_g_paths=1;
				g_paths=new char *[1];
				g_paths[i]=new char[NAME_MAX];
				sprintf(g_paths[i++],"graphics\\%s\\",strupr(finfo.cFileName));
			} /* if */ 

			while(FindNextFile(h,&finfo)==TRUE) {

				if (strcmp(finfo.cFileName,".")!=0 &&
					strcmp(finfo.cFileName,"..")!=0) {
					if (n_g_paths==0) {
						i=0;
						n_g_paths=1;
						g_paths=new char *[1];
						g_paths[i]=new char[NAME_MAX];
						sprintf(g_paths[i++],"graphics\\%s\\",strupr(finfo.cFileName));
					} else {
						tmp_g_paths=g_paths;
						n_g_paths++;
						g_paths=new char *[i+1];
						for(j=0;j<i;j++) {
							g_paths[j]=tmp_g_paths[j];
						} /* for */ 
						delete tmp_g_paths;
						g_paths[i]=new char[NAME_MAX];
						sprintf(g_paths[i++],"graphics\\%s\\",strupr(finfo.cFileName));
					} /* if */ 
				} /* if */ 
			} /* while */ 
		} else {
			g_paths=new char *[2];
			g_paths[0]=new char[NAME_MAX];
			g_paths[1]=new char[NAME_MAX];
			strcpy(g_paths[0],"graphics\\original\\");
			strcpy(g_paths[1],"graphics\\alternate\\");
		} /* if */ 

		act_g_path=0;
		g_path=g_paths[act_g_path];

	} /* if */ 


	if (s_paths==NULL) {

		WIN32_FIND_DATA finfo;
		HANDLE h;
		int i,j;
		char **tmp_s_paths; 

		n_s_paths=0;
		h=FindFirstFile("sound\\*.*",&finfo);
		if (h!=INVALID_HANDLE_VALUE) {
			if (strcmp(finfo.cFileName,".")!=0 &&
				strcmp(finfo.cFileName,"..")!=0) {
				i=0;
				n_s_paths=1;
				s_paths=new char *[1];
				s_paths[i]=new char[NAME_MAX];
				sprintf(s_paths[i++],"sound\\%s\\",strupr(finfo.cFileName));
			} /* if */ 

			while(FindNextFile(h,&finfo)==TRUE) {

				if (strcmp(finfo.cFileName,".")!=0 &&
					strcmp(finfo.cFileName,"..")!=0) {
					if (n_s_paths==0) {
						i=0;
						n_s_paths=1;
						s_paths=new char *[1];
						s_paths[i]=new char[NAME_MAX];
						sprintf(s_paths[i++],"sound\\%s\\",strupr(finfo.cFileName));
					} else {
						tmp_s_paths=s_paths;
						n_s_paths++;
						s_paths=new char *[i+1];
						for(j=0;j<i;j++) {
							s_paths[j]=tmp_s_paths[j];
						} /* for */ 
						delete tmp_s_paths;
						s_paths[i]=new char[NAME_MAX];
						sprintf(s_paths[i++],"sound\\%s\\",strupr(finfo.cFileName));
					} /* if */ 
				} /* if */ 
			} /* while */ 
		} else {
			s_paths=new char *[2];
			s_paths[0]=new char[NAME_MAX];
			s_paths[1]=new char[NAME_MAX];
			strcpy(s_paths[0],"sound\\original\\");
			strcpy(s_paths[1],"sound\\alternate\\");
		} /* if */ 

		act_s_path=0;
		s_path=s_paths[act_s_path];
	} /* if */ 
} /* init_paths */ 
#else
void init_paths(void) {

  int i,j;
  DIR *dd;
  struct dirent *direntry;
  
  if (g_paths==NULL) {	  
	  char **tmp_g_paths; 

	  n_g_paths=0;
	  
	  if((dd=opendir(ROOTDIR_PREFIX "graphics"))==NULL) {
	    output_debug_message("ERROR: Could not open directory " ROOTDIR_PREFIX "graphics\n");
	    sleep(5); exit(1);
	  }
	  if ((direntry=readdir(dd))==NULL) {
	    output_debug_message("ERROR: Could not read directory " ROOTDIR_PREFIX "graphics\n");
	    sleep(5); exit(1);
	  }
	  
	  while(direntry != NULL) {

	    // fprintf(stderr,"DIR: %s\n",direntry->d_name);
        output_debug_message("graphics subdirectory found: %s\n", direntry->d_name);
	    
	    if (strcmp(direntry->d_name,".")!=0 &&
		strcmp(direntry->d_name,"..")!=0) {
	      if (n_g_paths==0) {
		i=0;
		n_g_paths=1;
		g_paths=new char *[1];
		g_paths[i]=new char[NAME_MAX];
		sprintf(g_paths[i++],ROOTDIR_PREFIX "graphics/%s/",direntry->d_name);
	      } else {
		tmp_g_paths=g_paths;
		n_g_paths++;
		g_paths=new char *[i+1];
		for(j=0;j<i;j++) {
		  g_paths[j]=tmp_g_paths[j];
		} /* for */ 
		delete tmp_g_paths;
		g_paths[i]=new char[NAME_MAX];
		sprintf(g_paths[i++],ROOTDIR_PREFIX "graphics/%s/",direntry->d_name);        
	      } /* if */ 
	    } /* if */ 
	    
	    direntry=readdir(dd);
	  } /* while */ 
	  closedir(dd);
	  
	  if (n_g_paths==0) {
	    output_debug_message("ERROR: No directories for graphics found in graphics/* \n");
	    sleep(5); exit(1);
	  }
  } /* if */ 

  if (s_paths==NULL) {	  
	  char **tmp_s_paths; 

	  n_s_paths=0;
	  
	  if((dd=opendir(ROOTDIR_PREFIX "sound"))==NULL) {
	    output_debug_message("ERROR: Could not open directory " ROOTDIR_PREFIX "sound\n");
	    sleep(5); exit(1);
	  }
	  if ((direntry=readdir(dd))==NULL) {
	    output_debug_message("ERROR: Could not read directory " ROOTDIR_PREFIX "sound\n");
	    sleep(5); exit(1);
	  }
	        
	  while(direntry != NULL) {

	    // fprintf(stderr,"DIR: %s\n",direntry->d_name);
        output_debug_message("sound subdirectory found: %s\n", direntry->d_name);
	    
	    if (strcmp(direntry->d_name,".")!=0 &&
		strcmp(direntry->d_name,"..")!=0) {
	      if (n_s_paths==0) {
		i=0;
		n_s_paths=1;
		s_paths=new char *[1];
		s_paths[i]=new char[NAME_MAX];
		sprintf(s_paths[i++],ROOTDIR_PREFIX "sound/%s/",direntry->d_name);
	      } else {
		tmp_s_paths=s_paths;
		n_s_paths++;
		s_paths=new char *[i+1];
		for(j=0;j<i;j++) {
		  s_paths[j]=tmp_s_paths[j];
		} /* for */ 
		delete tmp_s_paths;
		s_paths[i]=new char[NAME_MAX];
		sprintf(s_paths[i++],ROOTDIR_PREFIX "sound/%s/",direntry->d_name);
	      } /* if */ 
	    } /* if */ 
	    
	    direntry=readdir(dd);
	  } /* while */ 
	  closedir(dd);
	  
	  if (n_s_paths==0) {
	    output_debug_message("ERROR: No directories for sound found in sound/* \n");
	    sleep(5); exit(1);
	  }
  } /* if */
  
    // DEBUG MOREIRA
  /*for (int _i = 0; _i < n_g_paths; _i++) {
      output_debug_message("[init_paths] g_paths[%d]='%s'\n", _i, g_paths[_i]);    
  }
  for (int _i = 0; _i < n_s_paths; _i++) {
      output_debug_message("[init_paths] s_paths[%d]='%s'\n", _i, s_paths[_i]);    
  }*/   

} /* init_paths */ 

#endif
