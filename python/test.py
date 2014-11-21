from gnuradio import gr, gr_unittest
from gnuradio import blocks
from numpy import array
import mspsa430

class qa_mspsa430 (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_mspsa430 (self):
        expected_result = [0]*129
        src = mspsa430.source ("")
        dst = blocks.vector_sink_b ();
        self.tb.connect (src, dst)
        self.tb.run ()
        result_data = dst.data ()
        #self.assertTupleEqual (expected_result, result_data)
        self.assertEqual (len(expected_result), len(result_data))

if __name__ == '__main__':
    gr_unittest.run (qa_mspsa430, "test.xml")
