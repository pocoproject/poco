//version 3.0.2

Ext.ux.Multiselect = Ext.extend(Ext.form.Field,  {
	store:null,
	dataFields:[],
	data:[],
	width:100,
	height:100,
	displayField:0,
	valueField:1,
	allowBlank:true,
	minLength:0,
	maxLength:Number.MAX_VALUE,
	blankText:Ext.form.TextField.prototype.blankText,
	minLengthText:'Minimum {0} item(s) required',
	maxLengthText:'Maximum {0} item(s) allowed',
	copy:false,
	allowDup:false,
	allowTrash:false,
	legend:null,
	focusClass:undefined,
	delimiter:',',
	view:null,
	dragGroup:null,
	dropGroup:null,
	tbar:null,
	initVal:null,
	appendOnly:false,
	sortField:null,
	sortDir:'ASC',
	defaultAutoCreate : {tag: "div"},
	
    initComponent: function(){
		Ext.ux.Multiselect.superclass.initComponent.call(this);
		this.addEvents({
			'dblclick' : true,
			'click' : true,
			'change' : true,
			'drop' : true,
			'rowselect' : true,
			'rowdeselect' : true
		});		
	},
    onRender: function(ct, position){
		var fs, cls, tpl;
		Ext.ux.Multiselect.superclass.onRender.call(this, ct, position);

		cls = 'ux-mselect';

		fs = new Ext.form.FieldSet({
			renderTo:this.el,
			title:this.legend,
			height:this.height,
			width:this.width,
			style:"padding:1px;",
			tbar:this.tbar
		});
		if(!this.legend){
			var x = fs.el.down('.'+fs.headerCls);
			if (x) x.remove();
		}
		fs.body.addClass(cls);

		tpl = '<tpl for="."><div class="' + cls + '-item';
		if(Ext.isIE || Ext.isIE7)tpl+='" unselectable=on';
		else tpl+=' x-unselectable"';
		tpl+='>{' + this.displayField + '}</div></tpl>';

		if(!this.store){
			this.store = new Ext.data.SimpleStore({
				fields: this.dataFields,
				data : this.data
			});
		}

		this.view = new Ext.ux.DDView({
			multiSelect: true, store: this.store, selectedClass: cls+"-selected", tpl:tpl,
			allowDup:this.allowDup, copy: this.copy, allowTrash: this.allowTrash, 
			dragGroup: this.dragGroup, dropGroup: this.dropGroup, itemSelector:"."+cls+"-item",
			isFormField:false, applyTo:fs.body, appendOnly:this.appendOnly,
			sortField:this.sortField, sortDir:this.sortDir
		});

		this.view.on('click', this.onViewClick, this);
		this.view.on('beforeClick', this.onViewBeforeClick, this);
		this.view.on('dblclick', this.onViewDblClick, this);
		this.view.on('drop', function(ddView, n, dd, e, data){
	    	return this.fireEvent("drop", ddView, n, dd, e, data);
		}, this);
		
		this.hiddenName = this.name;
		var hiddenTag={tag: "input", type: "hidden", value: "", name:this.name};
		if (this.isFormField) { 
			this.hiddenField = this.el.createChild(hiddenTag);
		} else {
			this.hiddenField = Ext.get(document.body).createChild(hiddenTag);
		}
		if (this.initVal != null)
		{
			this.setValue(this.initVal);
			this.initVal = null;
		}
		fs.doLayout();
	},
	
	initValue:Ext.emptyFn,
	
	onViewClick: function(vw, index, node, e) {
		var arrayIndex = this.preClickSelections.indexOf(index);
		if (arrayIndex  != -1)
		{
			this.preClickSelections.splice(arrayIndex, 1);
			this.view.clearSelections(true);
			this.view.select(this.preClickSelections);
			var sel = this.view.isSelected(arrayIndex);
			if (sel)
				this.fireEvent('rowselect', this, arrayIndex, sel);
			else
				this.fireEvent('rowdeselect', this, arrayIndex, sel);
		}
		this.fireEvent('change', this, this.getValue(), this.hiddenField.dom.value);
		this.hiddenField.dom.value = this.getValue();
		this.fireEvent('click', this, e);
		this.validate();		
	},

	onViewBeforeClick: function(vw, index, node, e) {
		this.preClickSelections = this.view.getSelectedIndexes();
		if (this.disabled) {return false;}
	},

	onViewDblClick : function(vw, index, node, e) {
		return this.fireEvent('dblclick', vw, index, node, e);
	},	
	
	getValue: function(valueField){
		var returnArray = [];
		var selectionsArray = this.view.getSelectedIndexes();
		if (selectionsArray.length == 0) {return '';}
		for (var i=0; i<selectionsArray.length; i++) {
			returnArray.push(this.store.getAt(selectionsArray[i]).get(((valueField != null)? valueField : this.valueField)));
		}
		return returnArray.join(this.delimiter);
	},

	setValue: function(values) {
		var index;
		var selections = [];
		this.view.clearSelections();
		this.hiddenField.dom.value = '';
		
		if (!values || (values == '')) { return; }
		
		if (!(values instanceof Array)) { values = values.split(this.delimiter); }
		for (var i=0; i<values.length; i++) {
			index = this.view.store.indexOf(this.view.store.query(this.valueField, 
				new RegExp('^' + values[i] + '$', "i")).itemAt(0));
			selections.push(index);
		}
		this.view.select(selections);
		this.hiddenField.dom.value = this.getValue();
		this.validate();
	},
	
	reset : function() {
		this.setValue('');
	},
	
	getRawValue: function(valueField) {
        var tmp = this.getValue(valueField);
        if (tmp.length) {
            tmp = tmp.split(this.delimiter);
        }
        else{
            tmp = [];
        }
        return tmp;
    },

    setRawValue: function(values){
        setValue(values);
    },

    validateValue : function(value){
        if (value.length < 1) { // if it has no value
             if (this.allowBlank) {
                 this.clearInvalid();
                 return true;
             } else {
                 this.markInvalid(this.blankText);
                 return false;
             }
        }
        if (value.length < this.minLength) {
            this.markInvalid(String.format(this.minLengthText, this.minLength));
            return false;
        }
        if (value.length > this.maxLength) {
            this.markInvalid(String.format(this.maxLengthText, this.maxLength));
            return false;
        }
        return true;
    },
	
    onDestroy : function(){
        if( this.view ) {
        this.view.destroy();
        }
        if (this.hiddenField) {
		this.hiddenField.remove();
		}
        Ext.ux.Multiselect.superclass.onDestroy.call(this);
    },
	
    onEnable : function(){
		this.el.unmask();
    },
	
    onDisable : function(){
		this.el.mask();
    }

});

Ext.reg("multiselect", Ext.ux.Multiselect);

Ext.ux.ItemSelector = Ext.extend(Ext.form.Field,  {
	msWidth:200,
	msHeight:300,
	hideNavIcons:false,
	imagePath:"",
	iconUp:"up2.gif",
	iconDown:"down2.gif",
	iconLeft:"left2.gif",
	iconRight:"right2.gif",
	iconTop:"top2.gif",
	iconBottom:"bottom2.gif",
	drawUpIcon:true,
	drawDownIcon:true,
	drawLeftIcon:true,
	drawRightIcon:true,
	drawTopIcon:true,
	drawBotIcon:true,
	fromStore:null,
	toStore:null,
	fromData:null, 
	toData:null,
	displayField:0,
	valueField:1,
	switchToFrom:false,
	allowDup:false,
	focusClass:undefined,
	delimiter:',',
	readOnly:false,
	toLegend:null,
	fromLegend:null,
	toSortField:null,
	fromSortField:null,
	toSortDir:'ASC',
	fromSortDir:'ASC',
	toTBar:null,
	fromTBar:null,
	bodyStyle:null,
	border:false,
	defaultAutoCreate:{tag: "div"},
	
    initComponent: function(){
		Ext.ux.ItemSelector.superclass.initComponent.call(this);
		this.addEvents({
			'rowdblclick' : true,
			'change' : true
		});			
	},

    onRender: function(ct, position){
		Ext.ux.ItemSelector.superclass.onRender.call(this, ct, position);

		this.fromMultiselect = new Ext.ux.Multiselect({
			legend: this.fromLegend,
			delimiter: this.delimiter,
			allowDup: this.allowDup,
			copy: this.allowDup,
			allowTrash: this.allowDup,
			dragGroup: this.readOnly ? null : "drop2-"+this.el.dom.id,
			dropGroup: this.readOnly ? null : "drop2-"+this.el.dom.id+",drop1-"+this.el.dom.id,
			width: this.msWidth,
			height: this.msHeight,
			dataFields: this.dataFields,
			data: this.fromData,
			displayField: this.displayField,
			valueField: this.valueField,
			store: this.fromStore,
			isFormField: false,
			tbar: this.fromTBar,
			appendOnly: true,
			sortField: this.fromSortField,
			sortDir: this.fromSortDir
		});
		this.fromMultiselect.on('dblclick', this.onRowDblClick, this);

		if (!this.toStore) {
			this.toStore = new Ext.data.SimpleStore({
				fields: this.dataFields,
				data : this.toData
			});
		}
		this.toStore.on('add', this.valueChanged, this);
		this.toStore.on('remove', this.valueChanged, this);
		this.toStore.on('load', this.valueChanged, this);

		this.toMultiselect = new Ext.ux.Multiselect({
			legend: this.toLegend,
			delimiter: this.delimiter,
			allowDup: this.allowDup,
			dragGroup: this.readOnly ? null : "drop1-"+this.el.dom.id,
			dropGroup: this.readOnly ? null : "drop2-"+this.el.dom.id+",drop1-"+this.el.dom.id,
			width: this.msWidth,
			height: this.msHeight,
			displayField: this.displayField,
			valueField: this.valueField,
			store: this.toStore,
			isFormField: false,
			tbar: this.toTBar,
			sortField: this.toSortField,
			sortDir: this.toSortDir
		});
		this.toMultiselect.on('dblclick', this.onRowDblClick, this);
				
		var p = new Ext.Panel({
			bodyStyle:this.bodyStyle,
			border:this.border,
			layout:"table",
			layoutConfig:{columns:3}
		});
		p.add(this.switchToFrom ? this.toMultiselect : this.fromMultiselect);
		var icons = new Ext.Panel({header:false});
		p.add(icons);
		p.add(this.switchToFrom ? this.fromMultiselect : this.toMultiselect);
		p.render(this.el);
		icons.el.down('.'+icons.bwrapCls).remove();

		if (this.imagePath!="" && this.imagePath.charAt(this.imagePath.length-1)!="/")
			this.imagePath+="/";
		this.iconUp = this.imagePath + (this.iconUp || 'up2.gif');
		this.iconDown = this.imagePath + (this.iconDown || 'down2.gif');
		this.iconLeft = this.imagePath + (this.iconLeft || 'left2.gif');
		this.iconRight = this.imagePath + (this.iconRight || 'right2.gif');
		this.iconTop = this.imagePath + (this.iconTop || 'top2.gif');
		this.iconBottom = this.imagePath + (this.iconBottom || 'bottom2.gif');
		var el=icons.getEl();
		if (!this.toSortField) {
			this.toTopIcon = el.createChild({tag:'img', src:this.iconTop, style:{cursor:'pointer', margin:'2px'}});
			el.createChild({tag: 'br'});
			this.upIcon = el.createChild({tag:'img', src:this.iconUp, style:{cursor:'pointer', margin:'2px'}});
			el.createChild({tag: 'br'});
		}
		this.addIcon = el.createChild({tag:'img', src:this.switchToFrom?this.iconLeft:this.iconRight, style:{cursor:'pointer', margin:'2px'}});
		el.createChild({tag: 'br'});
		this.removeIcon = el.createChild({tag:'img', src:this.switchToFrom?this.iconRight:this.iconLeft, style:{cursor:'pointer', margin:'2px'}});
		el.createChild({tag: 'br'});
		if (!this.toSortField) {
			this.downIcon = el.createChild({tag:'img', src:this.iconDown, style:{cursor:'pointer', margin:'2px'}});
			el.createChild({tag: 'br'});
			this.toBottomIcon = el.createChild({tag:'img', src:this.iconBottom, style:{cursor:'pointer', margin:'2px'}});
		}
		if (!this.readOnly) {
			if (!this.toSortField) {
				this.toTopIcon.on('click', this.toTop, this);
				this.upIcon.on('click', this.up, this);
				this.downIcon.on('click', this.down, this);
				this.toBottomIcon.on('click', this.toBottom, this);
			}
			this.addIcon.on('click', this.fromTo, this);
			this.removeIcon.on('click', this.toFrom, this);
		}
		if (!this.drawUpIcon || this.hideNavIcons) { this.upIcon.dom.style.display='none'; }
		if (!this.drawDownIcon || this.hideNavIcons) { this.downIcon.dom.style.display='none'; }
		if (!this.drawLeftIcon || this.hideNavIcons) { this.addIcon.dom.style.display='none'; }
		if (!this.drawRightIcon || this.hideNavIcons) { this.removeIcon.dom.style.display='none'; }
		if (!this.drawTopIcon || this.hideNavIcons) { this.toTopIcon.dom.style.display='none'; }
		if (!this.drawBotIcon || this.hideNavIcons) { this.toBottomIcon.dom.style.display='none'; }

		var tb = p.body.first();
		this.el.setWidth(p.body.first().getWidth());
		p.body.removeClass();
		
		this.hiddenName = this.name;
		var hiddenTag={tag: "input", type: "hidden", value: "", name:this.name};
		this.hiddenField = this.el.createChild(hiddenTag);
		this.valueChanged(this.toStore);
	},
	
	initValue:Ext.emptyFn,
	
	toTop : function() {
		if(this.disabled)return;
		var selectionsArray = this.toMultiselect.view.getSelectedIndexes();
		var records = [];
		if (selectionsArray.length > 0) {
			selectionsArray.sort();
			for (var i=0; i<selectionsArray.length; i++) {
				record = this.toMultiselect.view.store.getAt(selectionsArray[i]);
				records.push(record);
			}
			selectionsArray = [];
			for (var i=records.length-1; i>-1; i--) {
				record = records[i];
				this.toMultiselect.view.store.remove(record);
				this.toMultiselect.view.store.insert(0, record);
				selectionsArray.push(((records.length - 1) - i));
			}
		}
		this.toMultiselect.view.refresh();
		this.toMultiselect.view.select(selectionsArray);
	},

	toBottom : function() {
		if(this.disabled)return;
		var selectionsArray = this.toMultiselect.view.getSelectedIndexes();
		var records = [];
		if (selectionsArray.length > 0) {
			selectionsArray.sort();
			for (var i=0; i<selectionsArray.length; i++) {
				record = this.toMultiselect.view.store.getAt(selectionsArray[i]);
				records.push(record);
			}
			selectionsArray = [];
			for (var i=0; i<records.length; i++) {
				record = records[i];
				this.toMultiselect.view.store.remove(record);
				this.toMultiselect.view.store.add(record);
				selectionsArray.push((this.toMultiselect.view.store.getCount()) - (records.length - i));
			}
		}
		this.toMultiselect.view.refresh();
		this.toMultiselect.view.select(selectionsArray);
	},
	
	up : function() {
		if(this.disabled)return;
		var record = null;
		var selectionsArray = this.toMultiselect.view.getSelectedIndexes();
		selectionsArray.sort();
		var newSelectionsArray = [];
		if (selectionsArray.length > 0) {
			for (var i=0; i<selectionsArray.length; i++) {
				record = this.toMultiselect.view.store.getAt(selectionsArray[i]);
				if ((selectionsArray[i] - 1) >= 0) {
					this.toMultiselect.view.store.remove(record);
					this.toMultiselect.view.store.insert(selectionsArray[i] - 1, record);
					newSelectionsArray.push(selectionsArray[i] - 1);
				}
			}
			this.toMultiselect.view.refresh();
			this.toMultiselect.view.select(newSelectionsArray);
		}
	},

	down : function() {
		if(this.disabled)return;
		var record = null;
		var selectionsArray = this.toMultiselect.view.getSelectedIndexes();
		selectionsArray.sort();
		selectionsArray.reverse();
		var newSelectionsArray = [];
		if (selectionsArray.length > 0) {
			for (var i=0; i<selectionsArray.length; i++) {
				record = this.toMultiselect.view.store.getAt(selectionsArray[i]);
				if ((selectionsArray[i] + 1) < this.toMultiselect.view.store.getCount()) {
					this.toMultiselect.view.store.remove(record);
					this.toMultiselect.view.store.insert(selectionsArray[i] + 1, record);
					newSelectionsArray.push(selectionsArray[i] + 1);
				}
			}
			this.toMultiselect.view.refresh();
			this.toMultiselect.view.select(newSelectionsArray);
		}
	},
	
	fromTo : function() {
		if(this.disabled)return;
		var selectionsArray = this.fromMultiselect.view.getSelectedIndexes();
		var records = [];
		if (selectionsArray.length > 0) {
			for (var i=0; i<selectionsArray.length; i++) {
				record = this.fromMultiselect.view.store.getAt(selectionsArray[i]);
				records.push(record);
			}
			if(!this.allowDup)selectionsArray = [];
			for (var i=0; i<records.length; i++) {
				record = records[i];
				if(this.allowDup){
					var x=new Ext.data.Record();
					record.id=x.id;
					delete x;	
					this.toMultiselect.view.store.add(record);
				}else{
					this.fromMultiselect.view.store.remove(record);
					this.toMultiselect.view.store.add(record);
					selectionsArray.push((this.toMultiselect.view.store.getCount() - 1));
				}
			}
		}
		this.toMultiselect.view.refresh();
		this.fromMultiselect.view.refresh();
		if(this.toSortField)this.toMultiselect.store.sort(this.toSortField, this.toSortDir);
		if(this.allowDup)this.fromMultiselect.view.select(selectionsArray);
		else this.toMultiselect.view.select(selectionsArray);
	},
	
	toFrom : function() {
		if(this.disabled)return;
		var selectionsArray = this.toMultiselect.view.getSelectedIndexes();
		var records = [];
		if (selectionsArray.length > 0) {
			for (var i=0; i<selectionsArray.length; i++) {
				record = this.toMultiselect.view.store.getAt(selectionsArray[i]);
				records.push(record);
			}
			selectionsArray = [];
			for (var i=0; i<records.length; i++) {
				record = records[i];
				this.toMultiselect.view.store.remove(record);
				if(!this.allowDup){
					this.fromMultiselect.view.store.add(record);
					selectionsArray.push((this.fromMultiselect.view.store.getCount() - 1));
				}
			}
		}
		this.fromMultiselect.view.refresh();
		this.toMultiselect.view.refresh();
		if(this.fromSortField)this.fromMultiselect.store.sort(this.fromSortField, this.fromSortDir);
		this.fromMultiselect.view.select(selectionsArray);
	},
	
	valueChanged: function(store) {
		var record = null;
		var values = [];
		for (var i=0; i<store.getCount(); i++) {
			record = store.getAt(i);
			values.push(record.get(this.valueField));
		}
		this.hiddenField.dom.value = values.join(this.delimiter);
		this.fireEvent('change', this, this.getValue(), this.hiddenField.dom.value);
	},
	
	getValue : function() {
		return this.hiddenField.dom.value;
	},
	
	onRowDblClick : function(vw, index, node, e) {
		return this.fireEvent('rowdblclick', vw, index, node, e);
	},
	
	reset: function(){
		range = this.toMultiselect.store.getRange();
		this.toMultiselect.store.removeAll();
		if (!this.allowDup) {
			this.fromMultiselect.store.add(range);
			this.fromMultiselect.store.sort(this.displayField,'ASC');
		}
		this.valueChanged(this.toMultiselect.store);
	},

    onDestroy : function(){
        if( this.fromMultiselect ) {
		this.fromMultiselect.destroy();
        }
        if( this.toMultiselect ) {
		this.toMultiselect.destroy();
        }
        Ext.ux.ItemSelector.superclass.onDestroy.call(this);
	},
	
    onEnable : function(){
		this.fromMultiselect.enable();
		this.toMultiselect.enable();
    },
	
    onDisable : function(){
		this.fromMultiselect.disable();
		this.toMultiselect.disable();
    }
	
});

Ext.reg("itemselector", Ext.ux.ItemSelector);