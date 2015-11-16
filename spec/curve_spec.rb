require_relative '../curve'

describe Linear do
  context 'when going forward' do
    let :linear do
      Linear.new 100, 200
    end

    it 'should start with initial value' do
      expect(linear.get).to be 100
    end

    it 'should advance with time' do
      expect(linear.advance(5).get).to be 150
    end

    it 'should stop at the target position' do
      expect(linear.advance(15).get).to be 200
    end
  end

  context 'when going backward' do
    let :linear do
      Linear.new 100, 0
    end

    it 'should advance with time' do
      expect(linear.advance(5).get).to be 50
    end

    it 'should stop at the target position' do
      expect(linear.advance(15).get).to be 0
    end
  end
end
