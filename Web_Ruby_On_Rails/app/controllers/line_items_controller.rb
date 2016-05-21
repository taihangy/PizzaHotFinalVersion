class LineItemsController < ApplicationController
  skip_before_filter :authorize, only: [:create, :decrement, :increment]
  before_action :set_line_item, only: [:show, :edit, :update, :destroy]

  # GET /line_items
  # GET /line_items.json
  def index
    @line_items = LineItem.all
  end

  # GET /line_items/1
  # GET /line_items/1.json
  def show
  end

  # GET /line_items/new
  def new
    @line_item = LineItem.new
  end

  # GET /line_items/1/edit
  def edit
  end

  # POST /line_items
  # POST /line_items.json
  def create

    @cart = current_cart
    product = Product.find(params[:product_id])
    # build is alias of new
    # won't "create" a record in database, just create a new object in memory so that the view can take this object and display something, especially for a form.
    # not to trust param. Instead, verify it by finding in db at first.
    @line_item = @cart.add_product(product.id)

    respond_to do |format|
      if @line_item.save
        # session[:counter] = 0
        format.html { redirect_to @line_item.cart }
        # format.js { @current_item = @line_item }
        format.json { render :show, status: :created, location: @line_item }
      else
        format.html { render :new }
        format.json { render json: @line_item.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /line_items/1
  # PATCH/PUT /line_items/1.json
  def update
    respond_to do |format|
      if @line_item.update(line_item_params)
        format.html { redirect_to @line_item, notice: 'Line item was successfully updated.' }
        format.json { render :show, status: :ok, location: @line_item }
      else
        format.html { render :edit }
        format.json { render json: @line_item.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /line_items/1
  # DELETE /line_items/1.json
  def destroy
    @line_item.destroy
    respond_to do |format|
      format.html { redirect_to line_items_url, notice: 'Line item was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  def decrement
    @cart = current_cart
    @line_item = @cart.line_items.find_by_id(params[:id])

    @line_item.decrement
    respond_to do |format|
      if @line_item.save
        format.html { redirect_to @line_item.cart } 
        format.json { head :ok }
      else 
        format.html { render action: "edit" }
        format.json { render json: @line_item.errors, status: :unprocessable_entity }                         
      end
    end
  end

  def increment
    @cart = current_cart
    @line_item = @cart.line_items.find_by_id(params[:id])

    @line_item.increment
    respond_to do |format|
      if @line_item.save
        format.html { redirect_to @line_item.cart } 
        format.json { head :ok }
      else 
        format.html { render action: "edit" }
        format.json { render json: @line_item.errors, status: :unprocessable_entity }                         
      end
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_line_item
      @line_item = LineItem.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def line_item_params
      params.require(:line_item).permit(:product_id, :cart_id)
    end
end
