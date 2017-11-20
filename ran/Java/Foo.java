import java.lang.*;

public class Foo{
    int foo; //attribut public de classe, equivalent d'une variable globale en C
    public static void main(String[] args){
        Foo foo = new Foo();
        String s1 = "s1";
        String s2 = "s2";
        System.out.println("Vince "+ args.length);
        foo.foo = 2;
        //conversion string en entier
        try{
            foo.foo = Integer.parseInt(args[0]);
        }
        catch(NumberFormatException e){
            System.out.println(e + " " + args[0] + " is not an integer");
        }
        catch(ArrayIndexOutOfBoundsException e){
            System.out.println(e + " Argument is missing");
        }
        System.out.println(foo.foo);

        if(s1.equals(s2)){
            System.out.println("egalite");
        }
        else{
            System.out.println("differents");
        }
    }
}
