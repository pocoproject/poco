
Ext.grid.AppinfTable = Ext.extend(Ext.grid.EditorGridPanel, {
	/**
	 * @cfg {Boolean} autoEdit
     * enables autoEdit Mode
     */
     autoEdit: true,
     
     charThatStartedEdit: null,
     
     origEditValue: null,
     
      // private
    initComponent : function(){
    	//make sure we have cell selection!
    	this.selModel = new Ext.grid.CellSelectionModel();
      Ext.grid.AppinfTable.superclass.initComponent.call(this);
    },
    
     // private
    initEvents : function(){
        Ext.grid.AppinfTable.superclass.initEvents.call(this);
        this.on("keydown", this.onKeyDownEvent, this);
    },
    
    //private
    onKeyDownEvent: function(e){
		var code = e.getKey();
		if (code != undefined && code != e.LEFT && code != e.RIGHT && code != e.UP && code != e.DOWN && code != e.ESC && code != e.ENTER
			&& code != e.TAB && code != e.SHIFT && code != e.PAGEUP && code != e.PAGEDOWN && code != e.HOME && code != e.F5 &&
			code != e.END && code != e.DELETE && code != e.CONTROL && code != e.BACKSPACE){
			this.charThatStartedEdit = code;
			var sc = this.selModel.getSelectedCell();
			var col = sc[1];
			var row = sc[0];
			this.startEditing(row, col);
		}
		else
			this.charThatStartedEdit = null;
	},
});

Ext.reg('appinfgrid', Ext.grid.AppinfTable);

Ext.override(Ext.grid.AppinfTable, {
	startEditing : function(row, col){
		this.stopEditing();
		if(this.colModel.isCellEditable(col, row)){
			this.view.ensureVisible(row, col, true);
			var r = this.store.getAt(row);
			var field = this.colModel.getDataIndex(col);
			var e = {
				grid: this,
				record: r,
				field: field,
				value: r.data[field],
				row: row,
				column: col,
				cancel:false
			};
			if(this.fireEvent("beforeedit", e) !== false && !e.cancel){
				this.editing = true;
				var ed = this.colModel.getCellEditor(col, row);
				if(!ed.rendered){
					var edView = this.view.getEditorParent(ed);
					ed.render(edView);
				}
				var edfield = ed.field;
					ed.row = row;
					ed.col = col;
					ed.record = r;
					ed.field = edfield;
					ed.on("complete", this.onEditComplete, this, {single: true});
					ed.on("specialkey", this.selModel.onEditorKey, this.selModel);
					this.activeEditor = ed;
					var v = this.preEditValue(r, field);
					this.origEditValue = v;
					if (v == e.value)
					{
						if (this.autoEdit && this.charThatStartedEdit)
						{
							e.value = String.fromCharCode(this.charThatStartedEdit);
							ed.field.selectOnFocus = false;
						}
						else
							ed.field.selectOnFocus = true;
					}
					else
						ed.field.selectOnFocus = false;
					this.charThatStartedEdit = null;
					(function(){
					ed.startEdit(this.view.getCell(row, col), e.value);
					ed.field.preFocus();
				}).defer(50, this);
			}
		}
	},
	onEditComplete : function(ed, value, startValue){
        this.editing = false;
        this.activeEditor = null;
        ed.un("specialkey", this.selModel.onEditorKey, this.selModel);
		var r = ed.record;
        var field = this.colModel.getDataIndex(ed.col);
        value = this.postEditValue(value, startValue, r, field);
        if(String(value) !== String(this.origEditValue)){
            var e = {
                grid: this,
                record: r,
                field: field,
                originalValue: this.origEditValue,
                value: value,
                row: ed.row,
                column: ed.col,
                cancel:false
            };
            if(this.fireEvent("validateedit", e) !== false && !e.cancel){
                r.set(field, e.value);
                delete e.cancel;
                this.fireEvent("afteredit", e);
            }
        }
        this.view.focusCell(ed.row, ed.col);
    },
});
	 