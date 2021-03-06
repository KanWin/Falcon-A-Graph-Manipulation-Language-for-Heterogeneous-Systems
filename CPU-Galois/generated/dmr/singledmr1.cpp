
 #include "singledmr1.h"
 int   ntria ;


  int   changed , hchanged , pnnodes , pntriangles , success ;


  int   aborted ;


 

 

 HGraph  hgraph ;

 Galois::InsertBag<int>  WL1 ;
Galois::InsertBag<int>  WL2 ;
Galois::InsertBag<int>   *  current ;
Galois::InsertBag<int>   *  collnext ;
Galois::InsertBag<int>   *  colltemp ;
;


 void   readTriangle ( HGraph & hgraph ,char    *  name ) 
 {

 FILE*fp; 

 FILE*fp1; 

  char   temp [ 100] ;


  int   ntriaone , ntriatwo , n1 , n2 , n3 , n4 ;


  char    *  name1 ,  *  name2 ;


 name1=(char *)malloc(sizeof(char )*100); 

 name2=(char *)malloc(sizeof(char )*100); 

 sprintf(name1,"%s.ele",name);


 sprintf(name2,"%s.poly",name);


 fp=fopen(name1,"r");


 fscanf(fp,"%d",&ntriaone);


 fgets(temp,100,fp);


 for (int   i =0;i<ntriaone;i++) {

 fscanf(fp,"%d%d%d%d",&n1,&n2,&n3,&n4);


 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].id=n1; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].nodes[0]=n2; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].nodes[1]=n3; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1].nodes[2]=n4; 

 fgets(temp,100,fp);


 }

 fclose(fp);


 fp1=fopen(name2,"r");


 fgets(temp,100,fp1);


 fscanf(fp1,"%d",&ntriatwo);


 fgets(temp,100,fp1);


 for (int   i =0;i<ntriatwo;i++) {

 fscanf(fp1,"%d%d%d%d",&n1,&n2,&n3,&n4);


 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[0]=n2; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[1]=n3; 

 ((struct struct_hgraph  *)(hgraph.extra))->triangle[n1+ntriaone].nodes[2]=INVALID; 

 fgets(temp,100,fp1);


 }

 fclose(fp1);


 }
int   addTriangle ( HGraph & graph ,int & p1 ,int & p2 ,int & p3 ) 
 {

  int   t1 ;


 int  tr1 ;


 t1=HADD(&(devngraphtriangle),1);


 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[0]=p1; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[1]=p2; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]=p3; 

 return t1;

 }
int   addTriangle1 ( HGraph & graph ,int & p1 ,int & p2 ) 
 {

  int   t1 ;


 int  tr1 ;


 t1=HADD(&(devngraphtriangle),1);


 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[0]=p1; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[1]=p2; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]=INVALID; 

 return t1;

 }
void   verify ( int &  T ,HGraph & graph ) 
 {

  int   x ;


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[T].isdel==0&&((struct struct_hgraph  *)(graph.extra))->triangle[T].isbad==1  )
{
 changed=1; 
 }

 }
float   distanceSquare ( struct Point & p1 ,struct Point & p2 ,HGraph & graph ) 
 {

  float   x1 =p1.x-p2.x;


  float   y1 =p1.y-p2.y;


  float   dsqr =x1*x1+y1*y1;


 return dsqr;

 }
float   distance ( struct Point & one ,struct Point & two ,HGraph & graph ) 
 {

 return sqrtf(distanceSquare(one,two,graph));

 }
int  getOpposite ( int  & t1 ,HGraph & graph ,int   obtuse ) 
 {

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].nodes[2]==INVALID) ?2 : 3; 

  int   commonedgepoint1 =(obtuse+1)%dims;


 for (unsigned   ii =0;ii<dims;++ii) {

  int   nnedgestart =((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighedgestart[ii];


 if( nnedgestart==commonedgepoint1  )
{

 return t1;

 }

 }

 return INVALID;

 }
bool  copyNeighbours ( int  & t1 ,int  & t2 ,HGraph & graph ) 
 {

  int   dims ;


 for (int   i =0;i<3;i++) {

 ((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighbours[i]=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t2].neighbours[i]; 

 ((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighedgestart[i]=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t2].neighedgestart[i]; 

 }

 return false;

 }
bool  updateNeighbour ( int  & t1 ,int  & t2 ,int  & t3 ,HGraph & graph ) 
 {

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].nodes[2]==INVALID) ?2 : 3; 

 for (int   i =0;i<dims;i++) {

 if( ((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighbours[i]==t2  )
{

 ((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t1].neighbours[i]=t3; 

 }

 }

 return false;

 }
bool  checkbad (  int  & t ,HGraph & graph ) 
 {

 for (int   i =0;i<3;i++) {

 struct Point p1,p2,p3;

  int   te =(i+1)%3;


  int   te2 =(i+2)%3;


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[i]!=INVALID&&((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te]!=INVALID&&((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te2]!=INVALID  )
{

 int repli0=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[i];
p1.x=graph.points[2*repli0].fpe;
p1.y=graph.points[2*repli0+1].fpe;


 int repli1=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te];
p2.x=graph.points[2*repli1].fpe;
p2.y=graph.points[2*repli1+1].fpe;


 int repli2=((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[te2];
p3.x=graph.points[2*repli2].fpe;
p3.y=graph.points[2*repli2+1].fpe;


  float   vax =p2.x-p1.x;


  float   vay =p2.y-p1.y;


  float   vbx =p3.x-p1.x;


  float   vby =p3.y-p1.y;


  float   dp =vax*vbx+vay*vby;


 if( dp<0  )
((struct struct_hgraph  *)(graph.extra))->triangle[t].obtuse=i; 
else {

  float   dstsqrap1 =distanceSquare(p2,p1,graph);


  float   dstsqrap2 =distanceSquare(p3,p1,graph);


  float   c =dp/sqrtf(dstsqrap1*dstsqrap2);


 if( c>cos(30.000000*22.000000/(180.000000*7.000000))  )
return 1;

 }

 }

 }

 return 0;

 }
int   adjacent (  /**/int  t1 ,int  & t2 ,HGraph & graph ) 
 {

  int   common , firstmatch ;


 common=0; 

 firstmatch=3; 

  int   dimone , dimtwo ;


 dimone=(((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[2]==INVALID) ?2 : 3; 
 dimtwo=(((struct struct_hgraph  *)(graph.extra))->triangle[t2].nodes[2]==INVALID) ?2 : 3; 


 for (int   i =0;i<dimone;i++) {

 for (int   j =0;j<dimtwo;j++) {

 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t1].nodes[i]==((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t2].nodes[j]  )
{

 if( ++common==2  )
return firstmatch;
else firstmatch=i; 

 }

 }

 }

 return 3;

 }
void   dinit ( int &  t ,HGraph & graph ) 
 {

 ((struct struct_hgraph  *)(graph.extra))->triangle[t].obtuse=3; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t].isbad=checkbad(t,graph);


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[t].isbad==1  )
WL1.push(t); 

 ((struct struct_hgraph  *)(graph.extra))->triangle[t].isdel=0; 

 }
struct  cent  getCenter ( int  & t ,HGraph & graph ) 
 {

   struct cent   ce1 ;


 struct Point p1,p2,p3;

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->triangle[t].nodes[2]==INVALID) ?2 : 3; 

 int repli3=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t].nodes[0];
p1.x=graph.points[2*repli3].fpe;
p1.y=graph.points[2*repli3+1].fpe;


 int repli4=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t].nodes[1];
p2.x=graph.points[2*repli4].fpe;
p2.y=graph.points[2*repli4+1].fpe;


 if( dims==3  )
{
 int repli5=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t].nodes[2];
p3.x=graph.points[2*repli5].fpe;
p3.y=graph.points[2*repli5+1].fpe;

 }

 if( !(((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t].nodes[0]<((struct struct_hgraph  *)(graph.extra))->ntriangle&&((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t].nodes[1]<((struct struct_hgraph  *)(graph.extra))->ntriangle&&((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t].nodes[2]<((struct struct_hgraph  *)(graph.extra))->ntriangle)  )
{

 ce1.centerx=0.000000; 

 ce1.centery=0.000000; 

 return ce1;

 }

 if( dims==2  )
{

 ce1.centerx=(p1.x+p2.x)*0.500000; 

 ce1.centery=p1.y+p1.y*0.500000; 

 return ce1;

 }

  float   vax =p2.x-p1.x;


  float   vay =p2.y-p1.y;


  float   vbx =p3.x-p1.x;


  float   vby =p3.y-p1.y;


  float   xxlen =distance(p1,p2,graph);


  float   yylen =distance(p1,p3,graph);


  float   cosine =(vax*vbx+vay*vby)/(xxlen*yylen);


  float   sinesq =1.000000-cosine*cosine;


  float   plen =yylen/xxlen;


  float   ss =plen*cosine;


  float   tt =plen*sinesq;


  float   wp =(plen-cosine)/(2.000000*tt);


  float   wb =0.500000-(wp*ss);


 ce1.centerx=p1.x*(1.000000-wb-wp)+p2.x*wb+p3.x*wp; 

 ce1.centery=p1.y*(1.000000-wb-wp)+p2.y*wb+p3.y*wp; 

 return ce1;

 }
float   radiusSqaure ( struct Point & p1 ,int  & t ,HGraph & graph ) 
 {

 struct Point p2;

 int repli6=((struct struct_hgraph  *)(graph.extra))->/*4*/triangle[t].nodes[0];
p2.x=graph.points[2*repli6].fpe;
p2.y=graph.points[2*repli6+1].fpe;


 return distanceSquare(p1,p2,graph);

 }
bool  inCircumCircle ( struct Point & P0 ,int  & t1 ,HGraph & graph ) 
 {

   struct cent   ce1 ;


 ce1=getCenter(t1,graph);


 struct Point P1,P2;

 P1.x=ce1.centerx; 

 P1.y=ce1.centery; 

  float   dd =distanceSquare(P1,P0,graph);


 return dd<=radiusSqaure(P1,t1,graph);

 }
template<typename Pusher0 ,typename Pusher1>
void   drefine ( int  unni ,HGraph & graph ,Pusher0 &current, Pusher1 &collnext  ) 
 {

   struct cent   cent_new ;


 struct dev_vec pred,frontier,post,conntria1,conntria2;
pred= create_coll_device(100);
frontier= create_coll_device(100);
post= create_coll_device(100);
conntria1= create_coll_device(100);
conntria2= create_coll_device(100);


 struct dev_vec conn1,conn2,conn3,conn4;
conn1= create_coll_device(100);
conn2= create_coll_device(100);
conn3= create_coll_device(100);
conn4= create_coll_device(100);


 int  ceelem ;


  int   t1 ;


 t1=unni; 

 int  t ;


 // this is a access where rhs needs to b converted to int 
t=t1;


  int   lchanged =0;


  int   successfull =0;


 if( !(((struct struct_hgraph  *)(graph.extra))->triangle[t].isdel)&&(((struct struct_hgraph  *)(graph.extra))->triangle[t].isbad)  )
{

 ceelem=t; 

  int   cebot ;


 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[t].obtuse; 

  int   itr =0;


 
 while(cebot<3&&++itr<50&&ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle)  { 

 ceelem=getOpposite(ceelem,graph,cebot);


 if( ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle  )
{

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

 }

 if( ceelem>=((struct struct_hgraph  *)(graph.extra))->ntriangle||((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].isdel  )
{

 ceelem=t; 

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

 cent_new=getCenter(ceelem,graph);


 struct Point P1;

 int P0;

 P1.x=cent_new.centerx; 

 P1.y=cent_new.centery; 

 add_coll_device(pred,ceelem); 

 add_coll_device(frontier,ceelem); 

 
 while(frontier.size>0)  { 

 int  curr ;


 get_elem_coll_device(frontier,curr); 

 if( curr>=((struct struct_hgraph  *)(graph.extra))->ntriangle  )
break;

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->triangle[curr].nodes[2]==INVALID) ?2 : 3; 

 for (int   i =0;i<dims;i++) {

 int  next ;


 // this is a access where rhs needs to b converted to int 
next=((struct struct_hgraph  *)(graph.extra))->triangle[curr].neighbours[i]; 

 if( next>=((struct struct_hgraph  *)(graph.extra))->ntriangle  )
break;

 if( next>=((struct struct_hgraph  *)(graph.extra))->ntriangle  )
break;

 if( ((struct struct_hgraph  *)(graph.extra))->triangle[next].isdel  )
continue;

  int   nextdims ;


 nextdims=(((struct struct_hgraph  *)(graph.extra))->triangle[next].nodes[2]==INVALID) ?2 : 3; 

 if( !(next!=ceelem&&dims==2&&nextdims==2)&&inCircumCircle(P1,next,graph)  )
{

 if( nextdims==2&&dims!=2  )
{

 frontier.size=0; 

 pred.size=0; 

 conn1.size=0; 

 conn2.size=0; 

 conntria1.size=0; 

 conntria2.size=0; 

 ceelem=next; 

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 itr=0; 

 
 while(cebot<3&&++itr<50&&ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle)  { 

 ceelem=getOpposite(ceelem,graph,cebot);


 if( ceelem<((struct struct_hgraph  *)(graph.extra))->ntriangle  )
{

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

 }

 if( ceelem>=((struct struct_hgraph  *)(graph.extra))->ntriangle||((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].isdel  )
{

 ceelem=next; 

 cebot=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].obtuse; 

 }

 cent_new=getCenter(ceelem,graph);


 P1.x=cent_new.centerx; 

 P1.y=cent_new.centery; 

 add_coll_device(pred,ceelem); 

 add_coll_device(frontier,ceelem); 

 }
else {
  unsigned   jj ;


 for (jj=0;jj<pred.size;jj++) {

 if( pred[jj]==next  )
break;

 }

 if( jj==pred.size  )
{

 add_coll_device(pred,next); 

 add_coll_device(frontier,next); 

 }

 }

 }
else {

  int   pi1 =((struct struct_hgraph  *)(graph.extra))->triangle[curr].neighedgestart[i];


 if( pi1>=3  )
continue;

 int p1=((struct struct_hgraph  *)(graph.extra))->triangle[curr].nodes[pi1];

 int p2=((struct struct_hgraph  *)(graph.extra))->triangle[curr].nodes[(pi1+1)%3];

  unsigned   jj ;


 for (jj=0;jj<conn1.size;jj+=1) {

 if( conn1[jj]==p1&&conn2[jj]==p2  )
break;

 }

 if( jj==conn1.size  )
{

 add_coll_device(conn1,p1); 

 add_coll_device(conn2,p2); 

 add_coll_device(conntria1,curr); 

 add_coll_device(conntria2,next); 

 }

 }

 }

 }

 }

bool backoff=false;
for(int i=0;i<pred.size;i++)
 	 ((struct struct_hgraph  *)(graph.extra))->owner[pred.D_Vec[i]]=t;
#pragma omp barrier
for(int i=0;i<pred.size;i++){
 	 if(( (struct struct_hgraph  *   )(graph.extra))->owner[pred.D_Vec[i]]<t){backoff=true;break;}
 	 else if(((struct struct_hgraph  *)(graph.extra))->owner[pred.D_Vec[i]]>t)((struct struct_hgraph  *)(graph.extra))->owner[pred.D_Vec[i]]=t; 
}
#pragma omp barrier
if(backoff==0){
 	 for(int i=0;i<pred.size;i++){
 	 	if(( (struct struct_hgraph  *   )(graph.extra))->owner[pred.D_Vec[i]]!=t1){backoff=true;break;
 	 	}
 	}
}

if(backoff==0)  {

 if( !(((struct struct_hgraph  *)(graph.extra))->triangle[t].isdel)&&(((struct struct_hgraph  *)(graph.extra))->triangle[t].isbad)  )
{

 ++successfull; 

 int p0;

  int   dims ;


 dims=(((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].nodes[2]==INVALID) ?2 : 3; 

 p0=graph.addPoint(cent_new.centerx,cent_new.centery); 

 if( dims==2  )
{

 int  tr1 , tr2 ;


 int inv_point;

 inv_point=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].nodes[0]; 

  int   nov20 ;


 nov20=addTriangle1(graph,p0,inv_point);


 // this is a access where rhs needs to b converted to int 
tr1=nov20;


 inv_point=((struct struct_hgraph  *)(graph.extra))->triangle[ceelem].nodes[1]; 

 nov20=addTriangle1(graph,p0,inv_point);


 // this is a access where rhs needs to b converted to int 
tr2=nov20;


 add_coll_device(post,tr1); 

 add_coll_device(post,tr2); 

 copyNeighbours(tr1,ceelem,graph);


 copyNeighbours(tr2,ceelem,graph);


 }

 for (unsigned   ii =0;ii<conn1.size;ii+=1) {

 int p1=conn1[ii];

 int p2=conn2[ii];

 int  connsrc ;


 connsrc=conntria1[ii]; 

 int  conndst ;


 conndst=conntria2[ii]; 

 int  newtri ;


  int   nov20 ;


 nov20=addTriangle(graph,p0,p1,p2);


 // this is a access where rhs needs to b converted to int 
newtri=nov20;


  int   jj1 ;


 for (jj1=0;jj1<pred.size;jj1+=1) {

 if( pred[jj1]==conndst  )
break;

 jj1++; 

 }

 int  newconn ;


 if( jj1==pred.size  )
newconn=conndst; 
else newconn=connsrc; 

  int   iinbr =0;


 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighbours[0]=newconn; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighedgestart[0]=1; 

 iinbr++; 

 for (unsigned   jj =0;jj<post.size;++jj) {

  int   commonedgestart =adjacent(post[jj],newtri,graph);


 if( commonedgestart<3&&iinbr<3  )
{

 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighbours[iinbr]=post[jj]; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[newtri].neighedgestart[iinbr]=commonedgestart; 

 ++iinbr; 

 int  temp1 ;


 temp1=post[jj]; 

 int  unx ;


 unx=post[jj]; 

 updateNeighbour(unx,newconn,newtri,graph);


 }

 }

 add_coll_device(post,newtri); 

 }

  unsigned   jj ;


 for (jj=0;jj<pred.size;++jj) {

 int  temp1 ;


 get_elem_coll_device(pred,temp1); 

 ((struct struct_hgraph  *)(graph.extra))->triangle[temp1].isdel=1; 

 }

 for (unsigned   ii =0;ii<post.size;++ii) {

 int  xyx ;


 get_elem_coll_device(post,xyx); 

 ((struct struct_hgraph  *)(graph.extra))->triangle[xyx].obtuse=3; 

 ((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isbad=checkbad(xyx,graph);


 if( ((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isbad==1  )
collnext.push(xyx); 

 lchanged|=((struct struct_hgraph  *)(graph.extra))->triangle[xyx].isbad; 

 }

 if( lchanged  )
{

 changed=1; 

 }

 }

 }else {

 if( !(((struct struct_hgraph  *)(graph.extra))->triangle[t].isdel)&&(((struct struct_hgraph  *)(graph.extra))->triangle[t].isbad)  )
{

 collnext.push(t); 

 lchanged=1; 

 ++aborted; 


 }

 }

 }
struct Process1 {
 Process1(){}
 void operator()(int &req){
  drefine(req,hgraph,*current,*collnext);
}
};

int   main ( int   argc ,char    *  argv [ ] ) 
 { LonestarStart(argc,argv,name,desc,url);
if(argc>2)FALC_THREADS=atoi(argv[2]);

  int   ntria ;


 

 hgraph.readNodes(argv[1],2); 
int hosthgraph=1;
hgraph.extra=(struct struct_hgraph  *)malloc(sizeof(struct struct_hgraph ));
 read_hgraph_pptysize(hgraph);
alloc_extra_hgraph(hgraph,hosthgraph,hgraph.npoints);


 readTriangle(hgraph,argv[1]);


 HGraph graph;

 current=&WL1; 

 collnext=&WL2; 

 int temp1;
temp1=((struct struct_hgraph  *)hgraph.extra)->ntriangle;
#pragma omp parallel num_threads(12)
 for(int i=0;i<temp1;i++)dinit(i,hgraph);


 
 while(1)  { 

 changed=0; 

 
 Galois::do_all_local(*current,Process1());


 colltemp=current; 

 current=collnext; 

 collnext=current; 

 if( current->empty()  )
break;

 if( changed==0  )
break;

 }

 int temp4;
temp4=((struct struct_hgraph  *)hgraph.extra)->ntriangle;
#pragma omp parallel num_threads(12)
 for(int i=0;i<temp4;i++)verify(i,hgraph);


 }
