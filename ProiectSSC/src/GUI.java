import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class GUI extends JFrame {
    GUI(){
        this.setLocation(120,120);
        this.setSize(700,400);
        this.setTitle("Cache Memory Benchmark");

        JLabel title=new JLabel("Cache Memory Benchmark");
        title.setFont(new Font("Serif", Font.BOLD, 24));

        JTextArea info=new JTextArea(10,43);
        info.setEditable(false);
        info.setBorder(BorderFactory.createLoweredBevelBorder());
        info.append("Your PC processor and cache info:\n");
        info.append(this.getPcInfo());
        JPanel text=new JPanel();
        info.setFont(new Font("Serif", Font.BOLD, 18));
        text.add(info);

        JButton readBtn=new JButton("Read test");
        JButton writeBtn=new JButton("Write test");
        JButton hitMissBtn=new JButton("Hit&Miss test");
        JPanel buttons=new JPanel();
        buttons.add(readBtn);
        readBtn.setFont(new Font("Serif", Font.BOLD, 18));
        readBtn.setPreferredSize(new Dimension(150,30));
        buttons.add(Box.createHorizontalStrut(20));
        buttons.add(writeBtn);
        writeBtn.setFont(new Font("Serif", Font.BOLD, 18));
        writeBtn.setPreferredSize(new Dimension(150,30));
        buttons.add(Box.createHorizontalStrut(20));
        buttons.add(hitMissBtn);
        hitMissBtn.setFont(new Font("Serif", Font.BOLD, 18));
        hitMissBtn.setPreferredSize(new Dimension(150,30));

        readBtn.addActionListener(e -> {
            Process p=null;
            try {
                p=new ProcessBuilder("..\\Proiect\\Release\\Proiect.exe","1").start();
                p.waitFor();
                new resultPage("Read Test Result:Microseconds per number of elements read",
                        new File("MicrosecondsAveragePerSizeReading.png"));

            } catch (IOException | InterruptedException ioException) {
                ioException.printStackTrace();
            }
        });
        writeBtn.addActionListener(e -> {
            Process p=null;
            try {
                p=new ProcessBuilder("..\\Proiect\\Release\\Proiect.exe","0").start();
                p.waitFor();

                new resultPage("Write Test Result:Microseconds per number of elements written",
                        new File("MicrosecondsAveragePerSizeWriting.png"));

            } catch (IOException | InterruptedException ioException) {
                ioException.printStackTrace();
            }
        });
        hitMissBtn.addActionListener(e->{
            Process p=null;
            try {
                p=new ProcessBuilder("..\\Proiect\\Release\\Proiect.exe","3").start();
                p.waitFor();

                new HitMissPage(new File("HitMiss.txt"));

            } catch (IOException | InterruptedException ioException) {
                ioException.printStackTrace();
            }
        });
        JPanel main=new JPanel();
        main.add(title);
        title.setAlignmentX(Box.CENTER_ALIGNMENT);
        main.add(text);
        main.add(buttons);
        main.setLayout(new BoxLayout(main,BoxLayout.Y_AXIS));

        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setContentPane(main);
        this.setVisible(true);
    }

    String getPcInfo(){
        StringBuilder s= new StringBuilder();
        Process p=null;
        try {
        p=new ProcessBuilder("..\\Proiect\\Release\\Proiect.exe","-1").start();
        p.waitFor();

        File info=new File("PCinfo.txt");
        Scanner myReader = null;
        myReader = new Scanner(info);

        while (myReader.hasNextLine()) {
            String data = myReader.nextLine();
            s.append(data).append('\n');
        }
        myReader.close();
    } catch (InterruptedException | IOException e) {
        e.printStackTrace();
    }
        return s.toString();
    }
    public static void main(String[] args) {
        new GUI();

    }

}

