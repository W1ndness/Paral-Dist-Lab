import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.Job;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

public class Accelerate {
    public static final String INPUT_PATH = "hdfs://node01:9000/acc/input";
    public static final String OUTPUT_PATH = "hdfs://node01:9000/acc/output";
    public static class MyMapper extends Mapper<LongWritable, Text, IntWritable, NullWritable> {
        private int ret = 0;

        @Override
        protected void map(LongWritable key, Text value, Mapper<LongWritable, Text, IntWritable, NullWritable>.Context context) throws IOException, InterruptedException {
            int val = Integer.parseInt(value.toString().trim());
            ret += val;
        }

        @Override
        protected void cleanup(Mapper<LongWritable, Text, IntWritable, NullWritable>.Context context) throws IOException, InterruptedException {
            context.write(new IntWritable(ret), NullWritable.get());
        }
    }

    public static class MyReducer extends Reducer<IntWritable, NullWritable, IntWritable, NullWritable> {
        private int res = 0;

        @Override
        protected void reduce(IntWritable key, Iterable<NullWritable> values, Reducer<IntWritable, NullWritable, IntWritable, NullWritable>.Context context) throws IOException, InterruptedException {
            int val = key.get();
            res += val;
        }

        @Override
        protected void cleanup(Reducer<IntWritable, NullWritable, IntWritable, NullWritable>.Context context) throws IOException, InterruptedException {
            context.write(new IntWritable(res), NullWritable.get());
        }
    }

    public static void main(String[] args) throws IOException,
            InterruptedException,
            ClassNotFoundException,
            URISyntaxException {
        Configuration conf = new Configuration();

        FileSystem fs = FileSystem.get(new URI(INPUT_PATH), conf);
        Path outPath = new Path(OUTPUT_PATH);
        if (fs.exists(outPath)) {
            fs.delete(outPath, true);
        }

        Job job = Job.getInstance(conf, "Accelerate");
        job.setJarByClass(Accelerate.class);
        job.setMapperClass(MyMapper.class);
        job.setReducerClass(MyReducer.class);

        job.setMapOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(NullWritable.class);

        job.setOutputKeyClass(IntWritable.class);
        job.setMapOutputValueClass(NullWritable.class);

        FileInputFormat.addInputPath(job, new Path("hdfs://node01:9000/acc/input"));
        FileOutputFormat.setOutputPath(job, new Path("hdfs://node01:9000/acc/output"));

        job.waitForCompletion(true);
    }
}
