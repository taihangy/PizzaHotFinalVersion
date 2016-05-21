class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  before_filter :authorize
  protect_from_forgery with: :exception

  	private

		def current_cart 
			cart = Cart.find(session[:cart_id])
		rescue ActiveRecord::RecordNotFound 
			cart = Cart.create 
			session[:cart_id] = cart.id
			cart
		end

		def increment_count
			session[:counter] ||= 0
			session[:counter] += 1
		end

		def current_user
			return unless session[:user_id]
			@current_user ||= User.find_by_id(session[:user_id])
		end
		def authorize
			redirect_to login_url, notice: "Please log in" unless current_user
		end


end
