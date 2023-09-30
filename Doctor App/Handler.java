import java.io.*;
import java.util.*;
import com.google.firebase.*;
import com.google.firebase.database.*;
import com.google.auth.oauth2.*;
class Handler
{
DoctorAdmin da;
FirebaseDatabase firedb;
DatabaseReference ref;
String dir;
Handler()
{
Scanner sc = new Scanner(System.in);
System.out.println("Enter Patient's Name");
String name=sc.next();
da=new DoctorAdmin();
da.enterDetails(name);
try
{
authenticateJson();
ref.child(name).addValueEventListener(new ValueEventListener()
{
public void onDataChange(DataSnapshot ds)
{
try
{
Integer bpm=ds.child("bpm").getValue(Integer.class);
if(bpm!=null)
{
da.pl.plotValue(bpm.intValue());
da.updateBPM(bpm.intValue());
}
}
catch(Exception e)
{
System.out.println("Exception");
}
}
public void onCancelled(DatabaseError de)
{
}
}
);
}
catch(Exception e)
{
}
}
void authenticateJson()throws Exception
{
File f = new File(System.getProperty("user.dir"),"json");
int indx;
String fbio="";
for(String x : f.list())
{
if(x.indexOf(".json")!=-1)
{
indx=x.indexOf("-firebase-");
fbio=x.substring(0,indx);
f=new File(f,x);
break;
}
}
fbio="https://"+fbio+"-default-rtdb.firebaseio.com";
FileInputStream serviceAccount = new FileInputStream(f.getAbsolutePath());
FirebaseOptions options = new FirebaseOptions.Builder()
.setCredentials(GoogleCredentials.fromStream(serviceAccount))
.setDatabaseUrl(fbio)
.build();
FirebaseApp.initializeApp(options);
firedb=FirebaseDatabase.getInstance();
ref=firedb.getReference();
}
}