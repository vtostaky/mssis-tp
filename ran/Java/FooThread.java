import java.lang.*;

//Classe FooThread dont le parent est la classe Thread
public class FooThread extends Thread{
	int foo; //attribut public de classe, equivalent d'une variable globale en C
	public static void main(String[] args){
		System.out.println("Vince");
	}
}
