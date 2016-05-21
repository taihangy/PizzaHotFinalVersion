class SessionsController < ApplicationController
  skip_before_filter :authorize
  def new
  	@user = User.new
    render layout: false
  end

  def create
  	user = User.find_by(name: params[:name])
  	if user and user.authenticate(params[:password])
  		session[:user_id] = user.id
  		redirect_to admin_url
  	else
  		redirect_to login_url
  	end
  end

  def destroy
  	reset_session
  	redirect_to store_url, notice: "You have log out"
  end
end
