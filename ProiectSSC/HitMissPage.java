import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class HitMissPage extends JFrame {
    HitMissPage(File f){
        this.setLocation(200,200);
        this.setSize(450,465);
        this.setTitle("Hit and miss test");

        JPanel window= new JPanel();

        JLabel[] lvl=new JLabel[3];
        lvl[0]=new JLabel("For L1 cache:");
        lvl[1]=new JLabel("For L1,L2 cache:");
        lvl[2]=new JLabel("For L1,L2,L3 cache:");

        JPanel[] lvls=new JPanel[3];
        String[] answers =new String[4];
        answers[0]="Out of ";
        answers[1]=" tests, ";
        answers[2]=" were hits and ";
        answers[3]=" were misses";

        window.setLayout(new BoxLayout(window,BoxLayout.Y_AXIS));

        try {
            Scanner s=new Scanner(f);
            int i=0,limits=100;
            while(s.hasNextLine()){
                String hits,misses,hitRatio,missRatio,averageMemoryAccessTime;
                String line=s.nextLine();
                String[] split=line.split(" ");
                lvls[i]=new JPanel();
                lvls[i].setBorder(BorderFactory.createLoweredBevelBorder());
                window.add(Box.createVerticalStrut(20));
                hits=split[0];
                misses=split[1];
                hitRatio=split[2];
                missRatio=split[3];
                averageMemoryAccessTime=split[4];

                lvls[i].setLayout(new BoxLayout(lvls[i],BoxLayout.Y_AXIS));
                lvl[i].setFont(new Font("Bold", Font.BOLD, 16));
                lvls[i].add(lvl[i]);
                JLabel hitLabel=new JLabel(answers[0]+limits+answers[1]+hits+answers[2]+misses+answers[3]);
                hitLabel.setFont(new Font("Bold", Font.BOLD, 14));
                lvls[i].add(hitLabel);

                JLabel missLabel=new JLabel("Hit ratio:"+hitRatio+"%");
                missLabel.setFont(new Font("Bold", Font.BOLD, 14));
                lvls[i].add(missLabel);

                JLabel missRatioLabel=new JLabel("Miss ratio:"+missRatio+"%");
                missRatioLabel.setFont(new Font("Bold", Font.BOLD, 14));
                lvls[i].add(missRatioLabel);

                JLabel averageMemoryAccessTimeLabel=new JLabel("averageMemoryAccessTime:"+averageMemoryAccessTime+" nanoseconds");
                averageMemoryAccessTimeLabel.setFont(new Font("Bold", Font.BOLD, 14));
                lvls[i].add(averageMemoryAccessTimeLabel);
                window.add(lvls[i]);
                lvls[i].setPreferredSize(new Dimension(420,100));

                lvls[i].setAlignmentX(Box.CENTER_ALIGNMENT);
                i++;
                limits*=10;
            }
            s.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        window.add(Box.createVerticalStrut(20));
        JButton ok=new JButton("OK");
        ok.addActionListener(e -> this.dispose());
        window.add(ok);
        ok.setAlignmentX(Box.CENTER_ALIGNMENT);

        this.setContentPane(window);
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        this.setVisible(true);
    }
}
