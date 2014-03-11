//
// udp_service_impl.cpp
//
// Author: 	Lutz Bichler
//
// This file is part of the BMW Some/IP implementation.
//
// Copyright 2013, 2024 Bayerische Motoren Werke AG (BMW).
// All rights reserved.
//

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include <vsomeip/endpoint.hpp>
#include <vsomeip_internal/udp_service_impl.hpp>

namespace ip = boost::asio::ip;

namespace vsomeip {

udp_service_impl::udp_service_impl(
		boost::asio::io_service &_service, const endpoint *_location)
	: service_impl<ip::udp, VSOMEIP_MAX_UDP_MESSAGE_SIZE>(_service),
	  socket_(_service) {
}

udp_service_impl::~udp_service_impl() {
}


void udp_service_impl::start() {
	receive();
}

void udp_service_impl::stop() {
}

void udp_service_impl::send_queued() {
	ip::udp::endpoint target(
			ip::address::from_string(current_queue_->first->get_address()),
			current_queue_->first->get_port());

	socket_.async_send_to(
		boost::asio::buffer(&current_queue_->second.front()[0],
							current_queue_->second.front().size()),
		target,
		boost::bind(&udp_service_base_impl::send_cbk, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void udp_service_impl::receive() {
	socket_.async_receive_from(
	        boost::asio::buffer(buffer_), remote_,
	        boost::bind(&participant_impl::receive_cbk, this,
	          boost::asio::placeholders::error,
	          boost::asio::placeholders::bytes_transferred));
}

void udp_service_impl::restart() {
	receive();
}

const uint8_t * udp_service_impl::get_buffer() const {
	return buffer_.data();
}

} // namespace vsomeip
