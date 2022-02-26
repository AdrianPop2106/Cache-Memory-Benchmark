import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class resultPage extends JFrame {
    resultPage(String title, File f){
        this.setLocation(5,5);
        this.setSize(1550,800);
        this.setTitle(title);

        JPanel graphPanel=new JPanel();
        BufferedImage myPicture;
        JLabel graph;
        try {
            myPicture = ImageIO.read(f);
            graph = new JLabel(new ImageIcon(myPicture));
            graphPanel.add(graph);
        } catch (IOException e) {
            e.printStackTrace();
        }
        JPanel legend=new JPanel();
        legend.setLayout(new BoxLayout(legend,BoxLayout.Y_AXIS));
        legend.add(new JLabel("Legend"));
        legend.add(new JLabel("x - Number of integers in test list"));
        legend.add(new JLabel("y - Microseconds"));
        graphPanel.add(legend);
        graphPanel.add(Box.createRigidArea(new Dimension(10,3)));

        JButton ok=new JButton("OK");
        ok.addActionListener(e -> this.dispose());

        this.getContentPane().add(graphPanel);
        this.getContentPane().add(new JPanel().add(ok));
        this.getContentPane().add(Box.createVerticalStrut(10));
        this.getContentPane().setLayout(new BoxLayout(this.getContentPane(),BoxLayout.Y_AXIS));

        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        this.setVisible(true);
    }
}
