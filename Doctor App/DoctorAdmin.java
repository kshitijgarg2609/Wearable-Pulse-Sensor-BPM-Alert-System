import java.awt.*;
import javax.swing.*;
class DoctorAdmin
{
Dimension dim;
static int w,h;
int width=800,height=400;
JFrame jf;
String patient_name_str;
JLabel patient_name;
JLabel bpm_label;
SeriesGraphComponent1 pl;
DoctorAdmin()
{
dim=(Toolkit.getDefaultToolkit()).getScreenSize();
w=(int)(dim.getWidth());
h=(int)(dim.getHeight());
jf=new JFrame("Doctor Admin");
jf.setBounds(((w-width)/2),((h-height)/2),width,height);
jf.setResizable(false);
jf.getContentPane().setLayout(null);
addComponents();
jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
jf.setVisible(true);
jf.repaint();
jf.revalidate();
}
void addComponents()
{
pl=new SeriesGraphComponent1("BPM");
pl.initChartPanel();
pl.setRange(40,120);
pl.cp.setBounds(27, 59, 697, 267);
jf.getContentPane().add(pl.cp);

patient_name = new JLabel("",SwingConstants.CENTER);
patient_name.setBackground(Color.CYAN);
patient_name.setOpaque(true);
patient_name.setBounds(27, 24, 250, 24);
jf.getContentPane().add(patient_name);
bpm_label = new JLabel("BPM : ",SwingConstants.CENTER);
bpm_label.setBackground(Color.MAGENTA);
bpm_label.setOpaque(true);
bpm_label.setBounds(332, 24, 137, 24);
jf.getContentPane().add(bpm_label);
}
void enterDetails(String a)
{
patient_name.setText("Patient Name : "+a);
patient_name_str=a;
}
void updateBPM(int a)
{
bpm_label.setText((a==-1)?("BPM : "):("BPM : "+a));
}
}