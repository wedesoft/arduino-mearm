require_relative '../curve'

describe Curve do
  let :curve do
    Curve.new 0
  end

  it 'should start with the given value' do
    expect(curve.pos).to be 0
  end

  it 'should stay constant over time' do
    expect(curve.advance(5).pos).to eq 0
  end

  context 'when moving forward' do
    let :curve do
      Curve.new(100, acceleration: 10).retarget 140
    end

    it 'should advance with time' do
      expect(curve.advance(1).pos).to eq 105
    end

    it 'should accelerate over time' do
      expect(curve.advance(1).advance(1).pos).to eq 120
      expect(curve.advance(2).pos).to eq 120
    end

    it 'should use the specified acceleration' do
      curve = Curve.new(0, acceleration: 20).retarget 200
      expect(curve.advance(1).pos).to eq 10
    end

    it 'should determine time of reversal' do
      expect(curve.reverse_time).to eq 2
      expect(curve.advance(1).reverse_time).to eq 1
      expect(curve.advance(2).reverse_time).to eq 0
    end

    it 'should determine time of reversal when moving backward' do
      curve = Curve.new(100, acceleration: 10).retarget 60
      expect(curve.reverse_time).to eq 2
      expect(curve.advance(1).reverse_time).to eq 1
      expect(curve.advance(2).reverse_time).to eq 0
    end

    it 'should decelerate before the target' do
      expect(curve.advance(3).pos).to eq 135
    end

    it 'should stop at the target' do
      expect(curve.advance(4).pos).to eq 140
      expect(curve.advance(5).pos).to eq 140
    end

    it 'should accomodate a reduced target' do
      expect(curve.advance(2).retarget(120).advance(0).pos).to eq 120
    end

    it 'should ramp down the speed when getting the reduced target' do
      expect(curve.advance(2).retarget(120).advance(2).pos).to eq 140
    end
  end

  context 'when moving backward' do
    let :curve do
      Curve.new(100, acceleration: 10).retarget 60
    end

    it 'should advance with time' do
      expect(curve.advance(1).pos).to eq 95
    end
  end
end
