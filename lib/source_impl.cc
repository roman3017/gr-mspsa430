/* -*- c++ -*- */
/*
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "source_impl.h"

namespace gr {
  namespace mspsa430 {

    source::sptr source::make(const std::string path)
    {
      return gnuradio::get_initial_sptr(new source_impl(path));
    }

    /*
     * The private constructor
     *
     * Makes a source block
     */
    source_impl::source_impl(const std::string path)
      : gr::sync_block("mspsa430_source",
						gr::io_signature::make(0, 0, 0),
						gr::io_signature::make(1, 1, sizeof(int8_t)))
   {
		set_output_multiple(129);
		set_fixed_rate(129);
        this->m = new mspsa430(&this->lld);
        this->m->connect("/dev/ttyACM0", 921600);
        std::cout << this->m->get_info() << std::endl;
        this->m->setup();
    }

    /*
     * Our virtual destructor.
     */
    source_impl::~source_impl()
    {
		std::cout << "disconnect" << std::endl;
        this->m->disconnect();
    }

    int
    source_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        int8_t *out = reinterpret_cast<int8_t *>(output_items[0]);
        int count = 0;

        std::cout << "noutput_items: " << noutput_items << std::endl;
        std::cout << "reminder: " << noutput_items%129 << std::endl;

        std::vector<int8_t> spectrum;
		while (count < noutput_items) {
			spectrum = this->m->get_spectrum_no_init();
			//std::cout << "size: " << spectrum.size() << std::endl;
			if (spectrum.size() < 3) continue;
			for (size_t i = 0; i < spectrum.size(); i++) {
				out[count++] = spectrum.at(i);
				if (count == noutput_items)
					break;
			}
		}

        std::cout << "count: " << count << std::endl;
        return noutput_items;
    }
  } /* namespace mspsa430 */
} /* namespace gr */

