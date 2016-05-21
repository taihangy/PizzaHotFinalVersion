class ContactsController < ApplicationController
	skip_before_filter :authorize, only: [:new, :create]
	before_action :set_order, only: [:show, :edit, :update, :destroy]

	def index
		@contacts = Contact.all
	end

	def new
		@contact = Contact.new
	end

	def show
	end

	def edit
	end

	def create
		@contact = Contact.new(contact_params)
		respond_to do |format|
	      if @contact.save
	        format.html { redirect_to new_contact_path, notice: 'Thank you for your feedback' }
	        format.json { render :show, status: :created, location: @contact }
	      else
	        format.html { render :new }
	        format.json { render json: @contact.errors, status: :unprocessable_entity }
	      end
    	end
	end

	def update
	end

	def destroy
	end

	private 
		# Use callbacks to share common setup or constraints between actions.
	    def set_contact
	      @contact = Contact.find(params[:id])
	    end

	    # Never trust parameters from the scary internet, only allow the white list through.
	    def contact_params
	      params.require(:contact).permit(:name, :email, :feedback)
	    end
end
