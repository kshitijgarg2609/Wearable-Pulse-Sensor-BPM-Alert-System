import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import org.jfree.chart.*;
import org.jfree.chart.axis.*;
import org.jfree.data.time.*;
import org.jfree.data.xy.*;
import org.jfree.chart.plot.*;
class SeriesGraphComponent1
{
//chart
JFreeChart fc;
ChartPanel cp;
//datasets
TimeSeries ts;
TimeSeriesCollection tsc;
//plotters and axis
int ar=5000;
ValueAxis domain,range;
XYPlot plot;
//
long tstamp=0;
boolean gflg=true;
boolean set_cut_off=false;
double cut_off=-1;
SeriesGraphComponent1(String a)
{
initChart(a);
}
void initChart(String a)
{
ts=new TimeSeries("",Millisecond.class);
//ts=new TimeSeries("");
tsc=new TimeSeriesCollection(ts);
fc=ChartFactory.createTimeSeriesChart(null,"5 seconds",a,tsc,false,false,false);
fc.setBorderVisible(true);
plot=fc.getXYPlot();
plot.setBackgroundPaint(Color.white);
plot.setDomainGridlinesVisible(true);
plot.setDomainGridlinePaint(Color.black);
plot.setRangeGridlinesVisible(true);
plot.setRangeGridlinePaint(Color.black);
plot.getRenderer().setSeriesPaint(0,Color.green);
domain=plot.getDomainAxis();
range=plot.getRangeAxis();
//range.setRange(0,1023);
//range.setAutoRange(false);
domain.setTickLabelsVisible(false);
domain.setFixedAutoRange(ar);
}
void initChartPanel()
{
cp=new ChartPanel(fc);
resetChart();
}
void resetChart()
{
tsc.removeAllSeries();
ts=new TimeSeries("",Millisecond.class);
//ts=new TimeSeries("");
tsc.addSeries(ts);
}
void plotValue(int a)
{
if(tstamp==System.currentTimeMillis())
{
return;
}
ts.add((new Millisecond()),a);
tstamp=System.currentTimeMillis();
}
void setRange(double a,double b)
{
range.setRange(a,b);
}
void setFixedAutoRange(int a)
{
ar=a;
domain.setFixedAutoRange(ar);
}
}