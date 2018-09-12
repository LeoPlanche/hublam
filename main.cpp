#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <string> 
#include <cmath>



#include "algo_BFS.hpp"
#include "algo.hpp"
#include "algo_clique.hpp"
#include "algo_interval.hpp"
#include "algo_clustering.hpp"
#include "algo_branches.hpp"
#include "algo_cycles.hpp"
#include "algo_chordless.hpp"
#include "GraphIO.hpp"
#include "test.hpp"
#include "algo_union.hpp"
#include "dirent.h"

using namespace std;



// normal graph 
// membership
// originalfile
// union type 
// quasi clique cut off
// laminar_index

int main(int, char * argv[]){
	srand(time(NULL));
	
	//time start
	unsigned long long nsec;
	long double sec;
         /* initialize random seed: */
        srand (time(NULL));

  /* generate secret number between 1 and 10: */
	//timespec start,end;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        
	//clock_gettime(CLOCK_REALTIME,&start);
	start = std::chrono::system_clock::now();

	graph g;
        rdmgraph g2;
	edges e;
	vector<int> assign;
        
       /*
        g2 = randomGraph(40000);
        cout << "n : " << g2.n << " m : " << g2.m << endl;
       tuple<graph,queue<int>,queue<queue<int>>,int> pair = randomHubLam(g2,1,3,5,6,30);
       g=get<0>(pair);          
*/
          
       /*    string str = "C:\\Users\\Public\\Thesee\\test\\jesus.csv"; 
    bool  b = write_graph(g,str.c_str());*/
   
        
  //   bool  b = read_graph(g,"C:\\Users\\Public\\Thesee\\enzymes_296.graphml");
        cout << "break"<<endl;
 //      bool  b = read_graph(g,"C:\\Users\\Public\\Thesee\\CC218");
     //   cout << "break"<<endl;
        
        
              ifstream is;
            is.open("C:\\Users\\Public\\Thesee\\CC1626");    
	if(!is.is_open()){
		cout << "Error\nCould not open " << endl;
		return EXIT_FAILURE;
	}


	//is.ignore(1024,'\n');

	map<int,int> m,m2;
	int x,y,c=0;
	int d=0;
	double id,ig2;
	while(is >> x >> y ){
		if(m.find(x) == m.end()){
			m[x] = c;
                        m2[c]=x;
			++c;
		}
		if(m.find(y) == m.end()){
			m[y] = c;
                        m2[c]=y;
			++c;
		}
		d++;

	}
        g.n = c;
        g.m = d;
        g.g.clear();
        g.g.resize(g.n);
     
	is.close();
             is.open("C:\\Users\\Public\\Thesee\\CC1626");    
      //  is.open("graphBio");
    //  is.ignore(1024,'\n');

	while(is >> x >> y ){       
		g.g[m[x]].push_back(m[y]);
                g.g[m[y]].push_back(m[x]);
                if(m[x]==2456 || m[x]==374)
                cout << m[x] << " etait " << x << endl;
	}
	//for(map<int,int>::iterator it = m.begin(); it != m.end(); ++it){
	//	of << it->first << " " << it->second << endl;
    is.close();

          //      if(!b)
       //   cout << "bug" <<endl;
     
     
     /*
      if (isKLamiar(g,8))
          cout << "k lam "<< endl;
      else
          cout << "not k lam" << endl; 
       */
       
       cout << "before first hub" << endl;
       int s = findFirstHub(g,2,3);
        cout << "first hub found : " << s << endl;
       queue<int> q = findhubs(g,2,3,s);
       cout << "nombre hubs "  << q.size() << endl;
       
       queue<int> copyQ = q;
      while (!copyQ.empty()){
       cout << copyQ.front() << endl;
       copyQ.pop();
       }
       
      queue<queue<int>> l = findlaminars(g,2,3,q);
      cout << "nombre de lam : " << l.size() << " sommet " << g.n << endl;
      
      
      
      
      queue<queue<int>> copyL = l;
      while (!copyL.empty()){
       queue<int> copyLL = copyL.front();
      while (!copyLL.empty()){
       cout << copyLL.front() << " ";
       copyLL.pop();
       }
       cout << endl;
       copyL.pop();
       }
      
      
        if(l.empty())  {
            cout << "l empty" << endl;
            return 0;
              }

    
        label lab = fillLabel(g,q,l,11);
        
      int* distExact = (int*) malloc(g.n*g.n * sizeof(int));
      int* distLabel = (int*) malloc(g.n*g.n * sizeof(int));
       if (distExact == NULL || distLabel == NULL) 
       {       
           return 0;
       }
  
      
          for(int i = 0; i<g.n; i++){
                for(int j = 0; j<g.n; j++){
                    distExact[i+g.n*j]=-1;
                }
      }
     for(int i = 0; i<g.n; i++){
         queue<int> q1;
         q1.push(i);
           int curr = q1.front();
           for (std::vector<int>::iterator it = g.g[curr].begin(); it != g.g[curr].end();it++){
             if(distExact[i+g.n*(*it)]==-1)       
              {
                  distExact[i+g.n*(*it)]=distExact[i+g.n*curr]+1;
                  q1.push(*it);
              }
            }
             q1.pop();
         }
      

       
      for(int i = 0; i<g.n; i++){
                for(int j = 0; j<=i; j++){
                    int min = lab.n;
                    if(get<4>(lab.L[i]) == get<4>(lab.L[j])  && !lab.hub[i] && !lab.hub[j] )
                  {          
                     min = get<2>(lab.L[j])+get<2>(lab.L[i])+fabs(get<3>(lab.L[j])-get<3>(lab.L[i])); 
                  }
                      else {     
              for(int k = 0 ; k<lab.H[i].size();k++){               
                     if(lab.H[i][k]+lab.H[j][k] <min){
                  min = lab.H[i][k]+lab.H[j][k];                    
                     }
                    }                                                           
              }
                     distLabel[i+g.n*j]= min;
                     distLabel[j+g.n*i]= min;
                }
      }      
      
   


    float average = 0;    
    float averagebis = 0;     
    int max = -1;
    int min = g.n;
     for(int i = 0; i<g.n; i++){
                for(int j = 0; j<=i; j++){
                    average = average + (float) distLabel[i+g.n*j] - (float) distExact[i+g.n*j];
                    averagebis = averagebis+(float) distExact[i+g.n*j];
                    if(distLabel[i+g.n*j] -distExact[i+g.n*j]>max)                   
                        max = distLabel[i+g.n*j] -distExact[i+g.n*j];   
                    
                      if(distLabel[i+g.n*j] -distExact[i+g.n*j]<min)
                         min = distLabel[i+g.n*j] -distExact[i+g.n*j];
                    
                 
                    
                 /*   if(distLabel[i+g.n*j] -distExact[i+g.n*j]<0)     
                    { cout << "i " << i <<" j " << j << endl;
                    cout <<distLabel[i+g.n*j] -distExact[i+g.n*j] << " dist lab " << distLabel[i+g.n*j] << endl;}                 
                    */        
                }
      }
    average = (2.*average)/( (float) g.n * ((float) g.n +1.));
    averagebis = (2.*averagebis)/( (float) g.n * ((float) g.n +1.));
    cout <<  max << " " <<average << " " << min <<  " " << averagebis << endl;
  free(distLabel);
   free(distExact);
      
         
      //graph r = computeQuotient(q,l,g.n);
  /*    for(int i=0; i<r.n;i++){
            for (std::vector<int>::iterator it = r.g[i].begin(); it != r.g[i].end();it++){
               cout << "vertex : " << i << "  " << *it << endl;
            }
      } */

   
     //  squelette g1 = simpleSquelette(g,q,5);
       
     //  b = write_squelette(g1,"C:\\Users\\Public\\Thesee\\test\\jesus2.dot");
  
    
    /* while (!q.empty()){
       cout << q.front() << endl;
       q.pop();
       } */
    
        //string s = "res";
       // char * c = (char*)s.c_str();
        
        
        // TOUT POUR AUTOMATISER LE PROCESS
   
   
 /*      
        char * nameFile = argv[1];
        
         int i=0;
        int k =0;
        int j =0;
        int last;
        while(nameFile[i]!='\0')
        {
            i++;
            j++;
            k++;
            if (nameFile[i]=='\\')
            {   last = j;
                j=0;
            }
            if (nameFile[i]=='.')
            {   
                k=0;
            }
        }
        char* nameDire = nameFile +i-j-last;
        char* nameGraph = nameFile + i-j;
        
        
    
        
        
        bool b = read_graph(g,nameFile);
                 if(!b)
          cout << "bug" <<endl;
        if(g.n!=0){
        int s = findFirstHub(g,4,8);
       queue<int> q = findhubs(g,4,8,s);       
      queue<queue<int>> l = findlaminars(g,4,8,q);
      graph r = computeQuotient(q,l,g.n);
      
        
        
        string nomDire(nameGraph);
        string str = "../graph/"+nomDire;
     b = write_graph(r,str.c_str());
     
        }
 
    */
     
    
    //    FIN AUTOMATISATION
        
        
        
        
   /* 
      char * nameFile2 = argv[2];
      DIR * dir;

          char * nameFile = argv[1];
        
         int i=0;
        int k =0;
        int j =0;
        int last;
        while(nameFile[i]!='\0')
        {
            i++;
            j++;
            k++;
            if (nameFile[i]=='\\')
            {   last = j;
                j=0;
            }
            if (nameFile[i]=='.')
            {   
                k=0;
            }
        }
        char* nameDire = nameFile +i-j-last;
        char* nameGraph = nameFile + i-j;
        
       
        
      //  char * nameFile = "C:\\Users\\leo\\Documents\\NetBeansProjects\\TheseAlgoSquelette\\resultats\\PSM22MDI_clean.fasta_PSM22MDI_clean.fasta.blastn.CC150";
     //   char * nameFile2 = "C:\\Users\\leo\\Documents\\NetBeansProjects\\TheseAlgoSquelette\\resultatsIso\\PSM22MDI_clean.fasta_PSM22MDI_clean.fasta.blastn.CC150_10";
    struct dirent* ent;
    bool iso =false;

        if ((dir = opendir (argv[2])) != NULL) {
  // print all the files and directories within directory 
         while (((ent = readdir (dir)) != NULL) && !iso) {
             if(ent->d_name[0]=='C')
             {
      char * nameFile2 = ent->d_name;
    
      string s1(nameFile2); 
      s1 = argv[2]+s1;
     nameFile2= (char*) s1.c_str(); 

      bool  b = read_graph(g2,nameFile2);
      b = read_graph(g,nameFile);

    if(isomorphique(g,g2)){
        cout << "is iso" << endl;
    iso = true;
    int i =0;
    int j=0;
        while(nameFile2[i]!='\0')
        {
            i++;        
            j++;
            if(nameFile2[i]=='_')
                j=0;
            
        }
    char* fin =  nameFile2-j;
    int a = atoi(fin);
    a++;
    char* newFin = (char*) malloc(sizeof(int));
    sprintf(newFin, "%d", a);
    string newNameFile2 = "";
    k=0;
    int l=0;
    while(nameFile2[k]!='\0')
    {
        if(k<i-j+1)
        {
           newNameFile2= newNameFile2+nameFile2[k];  
        }
        else
        {
           newNameFile2= newNameFile2+newFin[l];
           l++;
        }
        k++;
    }
    if(newFin[l]!='\0')
    newNameFile2= newNameFile2+newFin[l]; 
    

 
    const char *cstr = newNameFile2.c_str();
  int result= rename( nameFile2 , cstr );
  if ( result != 0 )
    perror( "Error renaming file" );    
    
 if(a<10) {
      ofstream of;
       of.open(cstr,std::ofstream::out|std::ofstream::app);
        if(!of.is_open()){
		cout << "Error\nCould not open " << cstr << endl;
		return false;
	}
       string s(nameGraph);
	 of << s << endl;
	
	of.close();
 }
          
    }
        
        
    }
             
         }
         
         if(!iso){
    string s(nameGraph);    
    string str = "resultatsIso/"+s+"_1";
    //string str = "resultatsIso/"+s+"_1";
   bool b = write_graph(g,str.c_str());
        }
         
         
        }
        
            closedir (dir);

            */
            
       
      /*  
        int tab[2];
        
       for (int i = 0; i<2; i++) {
            if(i!=0)
        debut = rand() %  g.n;
         
        BFSTree t = BFS_tree_full(g,debut);

        BFSTree tt = clean_treeV2(t,3);
        tab[i] = tt.cycle;
        
        }       */


      // squelette s = algoReel(g,3,5);// IMPORTANT CLEANING
       
   //     string nomDire(nameDire);
      //  cout << nomDire << " GROS FDP " << endl;
       // string str = "C:\\Users\\leo\\Desktop\\Data\\PSM22\\PSM22_80_90.tgz_\\PSM22_80_90\\resultatsColor"+nomDire;
       // string str = "resultatsColor/"+nomDire+".dot";
        
    //    bool b = write_squelette(s,str.c_str());
        
        
        
        
      //  FIN AUTOMATISATION PROCESS 
     /*  int hubs =0;
       for (int i =0; i< s.n ; i++){
           if (s.hub[i]==i){
               hubs++;
           cout << i << endl;
           }
       }
       cout << hubs << " " << g.n << endl;*/
        
     //  graph r = createRelique(s);
        
      /* for (int i=0; i<r.n ;  i++){
           int voisins = 0;
           int voisin1 =0;
           int voisin2 =0;
       for(vector<int>::iterator it = r.g[i].begin(); it != r.g[i].end(); ++it){
            voisins++;
            voisin2 = voisin1;
            voisin1 = *it;
       }
           if (voisins ==2){                           
               r.g[i].clear();
            for(vector<int>::iterator it = r.g[voisin1].begin(); it != r.g[voisin1].end(); ++it){
                if(*it == voisin2)
                }
               
               
           }
       }*/
       
       
 /*     for (int i=0; i<r.n ;  i++){
       for(vector<int>::iterator it = r.g[i].begin(); it != r.g[i].end(); ++it){
           cout << i << " "<< *it << endl;
       }
       }*/
      
     
       /*
       for (int i= 0; i< r.n;i++){
             for (std::vector<int>::iterator it = r.g[i].begin(); it != r.g[i].end();it++){
                 if(i<*it)
                 {
                     cout << i << " " << *it << endl;
                 }
                 }
       }*/
        /*
           if(tab[0] != tab[1])
           {
        const char * input = "anomalie3.csv";
        ofstream of(input,ios::app);
        if(!of.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
          
        of  << tab[0] << ";" << tab[1] << ";" << debut <<";" <<nameDire << endl;

        of.close();
           }  
             */
      //  int seed = (int) argv[2];
        /*
        BFSTree t = BFS_tree_full(g,119);
        BFSTree tt = clean_tree(t,3);
        create_graph_from_tree(g2,tt);
        string str = "res.csv"; 
        b = write_graph(g2,str.c_str());
         */
       
        

        
    //  create_graph_from_tree(g2,tt); 
   
            
      
  //  b = write_graph(g2,"leBFS");
       // STATS
       /*
        struct caract a = create_caract(g2);
        int nbExtra  = tt.extraLink.size();
        if(g2.n != g.n){
        const char * input = "statSup3.csv";
        ofstream of(input,ios::app);
        if(!of.is_open()){
		cout << "Error\nCould not open " << input << endl;
		return false;
	}
	// of << g.n << " " << g.m << endl;
	//of.seekp(0, ios::end);
        float dens = (float)g.n/(float)g2.n;
          
        of  << a.deg1 <<";"<<  a.deg3 << ";" << a.degL << ";" << a.degMax << ";" << nbExtra << ";" << dens << ";" << g.n << ";" << g2.n << ";" <<nameGraph << endl;

        of.close();
    */  
        
        
      
        
        
        
        
        
        
        
        
        
       // create_graph_tree(BFSTree,g);
	
	/*if(!read_graph_coloring(g,e,nb,assign,argv[1],atoi(argv[2]))){
		return 1;
	}

	create_gephi_coloring(e,argv[3],nb,assign);

	string name(argv[3]);
	string name_without_ex = name.substr(0,name.find_last_of("."));

	find_cycles(g,e,nb,assign,name_without_ex);

	string union_name = name_without_ex + ".union.gdf";

	const char * union_char = union_name.c_str();
	
	create_gephi_coloring(e,union_char,nb,assign); */


	/*vector<vector<int> > cycles;
	chordless_cycles(g,cycles);

	//print_vv(cycles);
	//
	cout << cycles.size() << endl;
	for(int i=0;i<(int)cycles.size();++i){
		print_vector(cycles[i]);
		cout << endl;
	}*/

	//compactCommunities2(m.g,m1.g,m.e,100,nb,assign);

	//compactCommunities(m.g,m.e,100,nb,assign);
	//
	//newman_greedy_clustering(m.g,m.e,nb,assign);
	//nb = 2;	
	//if(branches(m.g,7,assign))
	//algo_cycles(m.g,2,3,assign);
	//unify(m.g,m.e,nb,assign);	
	//


	//testLexBFS(g);
	//testCliqueTree(g);
 	//testMultiSweep(m.g);
	//testIntervalGraph(m.g);
	//vector<int> assign;
	
	//cout << diametral_path(m.g,atoi(argv[2])) << endl;

	//create_gephi_coloring(m.e,argv[3],2,assign);
 	
	/*if(check_interval(m.g)){
		cout << "ok" << endl;
	}*/
	
	//vector<int> init = {1,4,3,6,7,2,5,0};
	//vector<int> init = {0,1,2,3,4,5,6,7};
	
	//testLexBFS_plus(g,init); 
	
	/*cout << diameter(m.g) << endl;

	clock_gettime(CLOCK_REALTIME,&end);

	nsec = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
	sec = nsec / 1e9;

	cout << fixed << setprecision(10) << sec << endl;

	clock_gettime(CLOCK_REALTIME,&start);

	
	cout << diameter_iFUB(m.g) << endl;*/
	/*if(diameter_check(m.g,10))
		cout << "ok" << endl;
	else
		cout << "bigger" << endl; */

	//time end
	//clock_gettime(CLOCK_REALTIME,&end);
      //   int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>
      //                       (end-start).count();
         
       //   std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	//nsec = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
	//sec = nsec / 1e9;

	//cout << fixed << setprecision(10) << sec << endl;
        //    std::cout << "finished computation at " << std::ctime(&end_time)
        //      << "elapsed time: " << elapsed_seconds << "s\n";
        
	return 0;
        }

