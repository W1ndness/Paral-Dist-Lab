import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import sun.print.PathGraphics;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

public class WordCount {
    public static final String INPUT_PATH = "hdfs://node01:9000/acc/input";
    public static final String OUTPUT_PATH = "hdfs://node01:9000/acc/output";

    public static class MyMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
        @Override
        protected void map(LongWritable key, Text value, Mapper<LongWritable, Text, Text, IntWritable>.Context context) throws IOException, InterruptedException {
            String line = value.toString();
            String[] words = line.split(" ");
            for (String word : words) {
                context.write(new Text(word), new IntWritable(1));
            }
        }
    }

    public static class MyReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
        @Override
        protected void reduce(Text key, Iterable<IntWritable> values, Reducer<Text, IntWritable, Text, IntWritable>.Context context) throws IOException, InterruptedException {
            int cnt = 0;
            for (IntWritable value : values) {
                cnt += value.get();
            }
            context.write(key, new IntWritable(cnt));
        }
    }

    public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException, URISyntaxException {
        Configuration conf = new Configuration();

        FileSystem fs = FileSystem.get(new URI(INPUT_PATH), conf);
        Path outPath = new Path(OUTPUT_PATH);
        if (fs.exists(outPath)) {
            fs.delete(outPath, true);
        }

        Job job = Job.getInstance(conf);

        job.setJarByClass(WordCount.class);

        job.setMapperClass(MyMapper.class);
        job.setReducerClass(MyReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(IntWritable.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path("hdfs://node01:9000/acc/input"));
        FileOutputFormat.setOutputPath(job, new Path("hdfs://node01:9000/acc/output"));

        job.waitForCompletion(true);
    }
}

